//
// Copyright (c) 2015 Gustavo Valiente gustavovalient@gmail.com
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#include "trjnode.h"

#include <algorithm>
#include "nanovg.h"
#include "trjapplication.h"
#include "trjrendercontext.h"
#include "trjdebug.h"

#ifdef TRJ_CFG_ENABLE_RENDER_CACHE
    #include "private/trjdisplaylistmanager.h"
#endif

namespace trj
{

Node::Node(const Node& other) :
    mPosition(other.mPosition),
    mBlendColor(other.mBlendColor),
    mScissorRect(other.mScissorRect),
    mBoundingBox(other.mBoundingBox),
    mFinalBoundingBox(other.mFinalBoundingBox),
    mTag(other.mTag),
    mRotationAngle(other.mRotationAngle),
    mSkewXAngle(other.mSkewXAngle),
    mSkewYAngle(other.mSkewYAngle),
    mScaleX(other.mScaleX),
    mScaleY(other.mScaleY),
    mOpacity(other.mOpacity),
    mBlendColorFactor(other.mBlendColorFactor),
    mActionsSpeed(other.mActionsSpeed),
    mFinalScaleX(other.mFinalScaleX),
    mFinalScaleY(other.mFinalScaleY),
    mVisible(other.mVisible),
    mHidden(other.mHidden),
    mScaleWithScreenAspectRatio(other.mScaleWithScreenAspectRatio),
    mFlipX(other.mFlipX),
    mFlipY(other.mFlipY),
    mActionsPaused(other.mActionsPaused),
    mRenderOffScreen(other.mRenderOffScreen),
    mInvalidateBoundingBox(other.mInvalidateBoundingBox),
    mInvalidateRenderCache(other.mInvalidateRenderCache || other.mRenderCache),
    mInvalidateTransform(other.mInvalidateTransform),
    mInvalidateHidden(other.mInvalidateHidden),
    mIsOnScreen(other.mIsOnScreen),
    mShapeGroups(other.mShapeGroups),
    mTransform(other.mTransform),
    mFinalTransform(other.mFinalTransform)
{
    int numActions = other.mActions.size();
    if(numActions)
    {
        mActions.reserve(numActions);

        for(const auto& action : other.mActions)
        {
            mActions.push_back(action->getClone());
        }
    }

    int numChildren = other.mChildren.size();
    if(numChildren)
    {
        mChildren.reserve(numChildren);

        for(const Node* child : other.mChildren)
        {
            auto childClone = child->getClone();
            childClone->mParent = this;
            mChildren.push_back(childClone.release());
        }
    }
}

Rect Node::generateBoundingBox()
{
    Rect boundingBox;

    for(ShapeGroup& shapeGroup : mShapeGroups)
    {
        boundingBox.join(shapeGroup.getBoundingBox());
    }

    return boundingBox;
}

void Node::updateItself(float)
{
}

bool Node::renderCacheAvailable(const RenderContext& renderContext) const
{
    return ! mFlipX && ! mFlipY && ! renderContext.isScissorEnabled();
}

void Node::renderItself(RenderContext& renderContext)
{
    for(const ShapeGroup& shapeGroup : mShapeGroups)
    {
        shapeGroup.render(renderContext);
    }
}

void Node::addActionImpl(Ptr<Action>&& action)
{
    TRJ_ASSERT(action.get(), "Action is empty");
    TRJ_ASSERT(! mActionsRunning, "Actions can't be modified while they are running");

    mActions.push_back(std::move(action));
}

void Node::addChildImpl(Ptr<Node>&& child)
{
    TRJ_ASSERT(child.get(), "Child is empty");

    Node* childPtr = child.release();
    childPtr->mParent = this;
    mChildren.push_back(childPtr);
}

void Node::insertChildImpl(int index, Ptr<Node>&& child)
{
    TRJ_ASSERT(child.get(), "Child is empty");
    TRJ_ASSERT(index >= 0 && index <= (int) mChildren.size(), "Invalid child index");

    Node* childPtr = child.release();
    childPtr->mParent = this;
    mChildren.insert(mChildren.begin() + index, childPtr);
}

void Node::setChildImpl(int index, Ptr<Node>&& child) noexcept
{
    TRJ_ASSERT(child.get(), "Child is empty");
    TRJ_ASSERT(index >= 0 && index < (int) mChildren.size(), "Invalid child index");

    Node* childPtr = child.release();
    childPtr->mParent = this;
    mChildren[index] = childPtr;
}

void Node::update(float elapsedTime, float actionsElapsedTime, bool actionsPaused)
{
    actionsPaused |= mActionsPaused;

    if(! actionsPaused)
    {
        mActionsRunning = true;
        actionsElapsedTime *= mActionsSpeed;

        for(auto it = mActions.begin(), end = mActions.end(); it != end; )
        {
            Action* action = it->get();
            if(action->run(actionsElapsedTime, *this))
            {
                it = mActions.erase(it);
                end = mActions.end();
            }
            else
            {
                ++it;
            }
        }

        mActionsRunning = false;
    }

    updateItself(elapsedTime);

    for(const Node* child : mChildren)
    {
        const_cast<Node*>(child)->update(elapsedTime, actionsElapsedTime, actionsPaused);
    }
}

void Node::render(RenderContext& renderContext)
{
    bool oldInvalidateFinalBoundingBoxes = renderContext.invalidateFinalBoundingBoxes();
    if(oldInvalidateFinalBoundingBoxes)
    {
        mInvalidateBoundingBox = true;
    }

    if(mInvalidateHidden)
    {
        mHidden = ! mVisible || isPositiveZero(mScaleX) || isPositiveZero(mScaleY) ||
                isPositiveZero(mOpacity) || (isPositive(mBlendColorFactor) && ! mBlendColor.isVisible());
        mInvalidateHidden = false;
    }

    if(mHidden)
    {
        if(oldInvalidateFinalBoundingBoxes)
        {
            for(const Node* child : mChildren)
            {
                const_cast<Node*>(child)->mInvalidateBoundingBox = true;
            }
        }
    }
    else
    {
        float oldFinalScaleX = renderContext.getFinalScaleX();
        float oldFinalScaleY = renderContext.getFinalScaleY();
        bool oldRenderOffScreen = renderContext.renderOffScreen();
        renderContext.setRenderOffScreen(oldRenderOffScreen || mRenderOffScreen);

        if(mInvalidateTransform || renderContext.windowSizeChanged())
        {
            updateTransform(renderContext);
            renderContext.setInvalidateFinalBoundingBoxes(true);
        }
        else
        {
            renderContext.setFinalScaleX(renderContext.getFinalScaleX() * mScaleX);
            renderContext.setFinalScaleY(renderContext.getFinalScaleY() * mScaleY);
        }

        bool invalidateFinalBoundingBoxes = renderContext.invalidateFinalBoundingBoxes();
        bool mustUpdateItself = mIsOnScreen || invalidateFinalBoundingBoxes ||
                renderContext.renderOffScreen();
        if(mustUpdateItself || ! mChildren.empty())
        {
            NVGcontext& nanoVgContext = renderContext.getNanoVgContext();
            nvgSave(&nanoVgContext);

            bool oldScissorEnabled = renderContext.isScissorEnabled();
            if(! mScissorRect.isEmpty())
            {
                nvgIntersectScissor(&nanoVgContext, mScissorRect.getX(), mScissorRect.getY(),
                        mScissorRect.getWidth(), mScissorRect.getHeight());
                renderContext.setScissorEnabled(true);
            }

            float* transform = mTransform.data();
            nvgTransform(&nanoVgContext, transform[0], transform[1], transform[2], transform[3],
                    transform[4], transform[5]);

            float oldOpacity = renderContext.getOpacity();
            float newOpacity = oldOpacity * mOpacity;
            renderContext.setOpacity(newOpacity);

            bool blendColorEnabled = isPositive(mBlendColorFactor);
            if(blendColorEnabled)
            {
                renderContext.pushBlendColor(mBlendColor, mBlendColorFactor);
            }

            if(invalidateFinalBoundingBoxes)
            {
                nvgCurrentTransform(&nanoVgContext, mFinalTransform.data());
                mFinalBoundingBox = getBoundingBox().getTransformed(mFinalTransform);
                mIsOnScreen = mFinalBoundingBox.isIntersecting(renderContext.getWindowRect());
            }

            if(mIsOnScreen || renderContext.renderOffScreen())
            {
                #ifdef TRJ_CFG_ENABLE_RENDER_CACHE
                    if(renderCacheAvailable(renderContext))
                    {
                        float finalScaleX = renderContext.getFinalScaleX();
                        float finalScaleY = renderContext.getFinalScaleY();

                        if(mInvalidateRenderCache || ! areEquals(mFinalScaleX, finalScaleX) ||
                                ! areEquals(mFinalScaleY, finalScaleY))
                        {
                            mFinalScaleX = finalScaleX;
                            mFinalScaleY = finalScaleY;
                            mInvalidateRenderCache = false;

                            if(mRenderCache)
                            {
                                nvgResetDisplayList(static_cast<NVGdisplayList*>(mRenderCache));
                            }
                            else
                            {
                                mRenderCache = &(priv::DisplayListManager::pull());
                            }

                            nvgSave(&nanoVgContext);
                            nvgResetTransform(&nanoVgContext);
                            nvgScale(&nanoVgContext, finalScaleX, finalScaleY);
                            nvgBindDisplayList(&nanoVgContext, static_cast<NVGdisplayList*>(mRenderCache));
                            renderItself(renderContext);
                            nvgBindDisplayList(&nanoVgContext, nullptr);
                            nvgRestore(&nanoVgContext);
                        }

                        nvgSave(&nanoVgContext);
                        nvgScale(&nanoVgContext, 1 / finalScaleX, 1 / finalScaleY);
                        nvgGlobalAlpha(&nanoVgContext, newOpacity);
                        nvgDrawDisplayList(&nanoVgContext, static_cast<NVGdisplayList*>(mRenderCache));
                        nvgRestore(&nanoVgContext);
                    }
                    else
                    {
                        releaseRenderCache();

                        nvgGlobalAlpha(&nanoVgContext, newOpacity);
                        renderItself(renderContext);
                    }
                #else
                    nvgGlobalAlpha(&nanoVgContext, newOpacity);
                    renderItself(renderContext);
                #endif

                if(renderContext.showBoundingBoxes() && ! mFinalBoundingBox.isEmpty())
                {
                    nvgSave(&nanoVgContext);
                    nvgResetTransform(&nanoVgContext);

                    nvgBeginPath(&nanoVgContext);
                    nvgRect(&nanoVgContext, mFinalBoundingBox.getX(), mFinalBoundingBox.getY(),
                            mFinalBoundingBox.getWidth(), mFinalBoundingBox.getHeight());
                    nvgStrokeWidth(&nanoVgContext, 1);
                    nvgStrokeColor(&nanoVgContext, nvgRGBAf(0, 0, 0, 0.5));
                    nvgStroke(&nanoVgContext);

                    nvgRestore(&nanoVgContext);
                }
            }

            for(const Node* child : mChildren)
            {
                const_cast<Node*>(child)->render(renderContext);
            }

            renderContext.setScissorEnabled(oldScissorEnabled);
            renderContext.setOpacity(oldOpacity);
            if(blendColorEnabled)
            {
                renderContext.popBlendColor();
            }

            nvgRestore(&nanoVgContext);
        }

        renderContext.setFinalScaleX(oldFinalScaleX);
        renderContext.setFinalScaleY(oldFinalScaleY);
        renderContext.setRenderOffScreen(oldRenderOffScreen);
    }

    renderContext.setInvalidateFinalBoundingBoxes(oldInvalidateFinalBoundingBoxes);
}

void Node::updateTransform(RenderContext& renderContext)
{
    float* transform = mTransform.data();
    nvgTransformIdentity(transform);
    mInvalidateTransform = false;

    if(mFlipX)
    {
        if(mFlipY)
        {
            const float otherTransform[6] = { -1, 0, 0, -1, 0, 0 };
            nvgTransformPremultiply(transform, otherTransform);
        }
        else
        {
            const float otherTransform[6] = { -1, 0, 0, 1, 0, 0 };
            nvgTransformPremultiply(transform, otherTransform);
        }
    }
    else if(mFlipY)
    {
        const float otherTransform[6] = { 1, 0, 0, -1, 0, 0 };
        nvgTransformPremultiply(transform, otherTransform);
    }

    float otherTransform[6];
    if(! isZero(mPosition.getX()) || ! isZero(mPosition.getY()))
    {
        nvgTransformTranslate(otherTransform, mPosition.getX(), mPosition.getY());
        nvgTransformPremultiply(transform, otherTransform);
    }

    float scaleX = mScaleX;
    if(mScaleWithScreenAspectRatio)
    {
        scaleX *= renderContext.getAspectRatio();
    }

    if(! areEquals(scaleX, 1) || ! areEquals(mScaleY, 1))
    {
        renderContext.setFinalScaleX(renderContext.getFinalScaleX() * scaleX);
        renderContext.setFinalScaleY(renderContext.getFinalScaleY() * mScaleY);

        nvgTransformScale(otherTransform, scaleX, mScaleY);
        nvgTransformPremultiply(transform, otherTransform);
    }

    if(! isZero(mRotationAngle))
    {
        nvgTransformRotate(otherTransform, mRotationAngle);
        nvgTransformPremultiply(transform, otherTransform);
    }

    if(! isZero(mSkewXAngle))
    {
        nvgTransformSkewX(otherTransform, mSkewXAngle);
        nvgTransformPremultiply(transform, otherTransform);
    }

    if(! isZero(mSkewYAngle))
    {
        nvgTransformSkewY(otherTransform, mSkewYAngle);
        nvgTransformPremultiply(transform, otherTransform);
    }
}

void Node::releaseRenderCache()
{
    #ifdef TRJ_CFG_ENABLE_RENDER_CACHE
        if(mRenderCache)
        {
            NVGdisplayList* displayList = static_cast<NVGdisplayList*>(mRenderCache);
            priv::DisplayListManager::push(*displayList);
            mRenderCache = nullptr;
            mInvalidateRenderCache = true;
        }
    #endif
}

Node& Node::getRootNode() noexcept
{
    return Application::getRootNode();
}

Node::~Node()
{
    clearChildren();
    releaseRenderCache();
}

const Rect& Node::getBoundingBox()
{
    if(mInvalidateBoundingBox)
    {
        mBoundingBox = generateBoundingBox();
        mInvalidateBoundingBox = false;
    }

    return mBoundingBox;
}

void Node::setRotationAngle(float angle) noexcept
{
    TRJ_ASSERT(angle >= 0 && angle < 2 * kPi, "Invalid angle");

    mRotationAngle = angle;
    invalidateTransform();
}

void Node::setScaleX(float scale) noexcept
{
    TRJ_ASSERT(scale >= 0, "Invalid scale x");

    mScaleX = scale;
    invalidateTransform();
    invalidateHidden();
}

void Node::setScaleY(float scale) noexcept
{
    TRJ_ASSERT(scale >= 0, "Invalid scale y");

    mScaleY = scale;
    invalidateTransform();
    invalidateHidden();
}

void Node::setOpacity(float opacity) noexcept
{
    TRJ_ASSERT(opacity >= 0 && opacity <= 1, "Invalid opacity");

    mOpacity = opacity;
    invalidateHidden();
    invalidateRenderCache();
}

void Node::setBlendColor(const Color& blendColor, float blendFactor) noexcept
{
    TRJ_ASSERT(blendFactor >= 0 && blendFactor <= 1, "Invalid blend factor");

    mBlendColor = blendColor;
    mBlendColorFactor = blendFactor;
    invalidateHidden();
    invalidateRenderCache();
}

void Node::reserveShapeGroups(int numShapeGroups)
{
    TRJ_ASSERT(numShapeGroups > 0, "Invalid num shape groups");

    mShapeGroups.reserve(numShapeGroups);
}

void Node::insertShapeGroup(int index, ShapeGroup shapeGroup)
{
    TRJ_ASSERT(index >= 0 && index <= (int) mShapeGroups.size(), "Invalid shape group index");

    mShapeGroups.insert(mShapeGroups.begin() + index, std::move(shapeGroup));
    invalidateBoundingBox();
}

void Node::setShapeGroup(int index, ShapeGroup shapeGroup) noexcept
{
    TRJ_ASSERT(index >= 0 && index < (int) mShapeGroups.size(), "Invalid shape group index");

    mShapeGroups[index] = std::move(shapeGroup);
    invalidateBoundingBox();
}

void Node::removeShapeGroup(int index) noexcept
{
    TRJ_ASSERT(index >= 0 && index < (int) mShapeGroups.size(), "Invalid shape group index");

    mShapeGroups.erase(mShapeGroups.begin() + index);
    invalidateBoundingBox();
}

ShapeGroup Node::releaseShapeGroup(int index) noexcept
{
    TRJ_ASSERT(index >= 0 && index < (int) mShapeGroups.size(), "Invalid shape group index");

    auto it = mShapeGroups.begin() + index;
    ShapeGroup releasedShapeGroup = std::move(*it);
    mShapeGroups.erase(it);
    invalidateBoundingBox();

    return releasedShapeGroup;
}

std::vector<ShapeGroup> Node::releaseShapeGroups() noexcept
{
    std::vector<ShapeGroup> releasedShapeGroups;
    mShapeGroups.swap(releasedShapeGroups);
    invalidateBoundingBox();

    return releasedShapeGroups;
}

void Node::reserveActions(int numActions)
{
    TRJ_ASSERT(numActions > 0, "Invalid num actions");

    mActions.reserve(numActions);
}

void Node::setActions(std::vector<Ptr<Action>>&& actions) noexcept
{
    TRJ_ASSERT(! mActionsRunning, "Actions can't be modified while they are running");

    #ifdef TRJ_DEBUG
        for(const auto& action : actions)
        {
            TRJ_ASSERT(action.get(), "Action is empty");
        }
    #endif

    mActions = std::move(actions);
}

void Node::clearActions() noexcept
{
    TRJ_ASSERT(! mActionsRunning, "Actions can't be modified while they are running");

    mActions.clear();
}

void Node::resetActions() noexcept
{
    TRJ_ASSERT(! mActionsRunning, "Actions can't be modified while they are running");

    std::vector<Ptr<Action>>().swap(mActions);
}

void Node::setActionsSpeed(float speed) noexcept
{
    TRJ_ASSERT(isPositive(speed), "Invalid speed");
    TRJ_ASSERT(! mActionsRunning, "Actions can't be modified while they are running");

    mActionsSpeed = speed;
}

void Node::setActionsPaused(bool paused) noexcept
{
    TRJ_ASSERT(! mActionsRunning, "Actions can't be modified while they are running");

    mActionsPaused = paused;
}

const Node& Node::getChild(int index) const noexcept
{
    return const_cast<Node&>(*this).getChild(index);
}

Node& Node::getChild(int index) noexcept
{
    TRJ_ASSERT(index >= 0 && index < (int) mChildren.size(), "Invalid child node index");

    const Node* child = mChildren[index];
    return const_cast<Node&>(*child);
}

const Node* Node::findChild(const String& tag) const noexcept
{
    return const_cast<Node*>(this)->findChild(tag);
}

Node* Node::findChild(const String& tag) noexcept
{
    for(const Node* child : mChildren)
    {
        if(child->mTag == tag)
        {
            return const_cast<Node*>(child);
        }
    }

    return nullptr;
}

const Node* Node::findChildRecursive(const String& tag) const noexcept
{
    return const_cast<Node*>(this)->findChild(tag);
}

Node* Node::findChildRecursive(const String& tag) noexcept
{
    for(const Node* child : mChildren)
    {
        if(child->mTag == tag)
        {
            return const_cast<Node*>(child);
        }
    }

    for(const Node* child : mChildren)
    {
        const Node* grandChild = child->findChildRecursive(tag);
        if(grandChild)
        {
            return const_cast<Node*>(grandChild);
        }
    }

    return nullptr;
}

void Node::reserveChildren(int numChildren)
{
    TRJ_ASSERT(numChildren > 0, "Invalid num children");

    mChildren.reserve(numChildren);
}

void Node::setChildren(std::vector<Ptr<Node>>&& children)
{
    mChildren.clear();
    mChildren.reserve(children.size());

    for(Ptr<Node>& child : children)
    {
        addChildImpl(std::move(child));
    }
}

void Node::removeChild(int index) noexcept
{
    TRJ_ASSERT(index >= 0 && index < (int) mChildren.size(), "Invalid child node index");

    delete mChildren[index];
    mChildren.erase(mChildren.begin() + index);
}

void Node::removeChild(Node& child) noexcept
{
    auto it = std::find(mChildren.begin(), mChildren.end(), &child);

    TRJ_ASSERT(it != mChildren.end(), "Child not found");

    removeChild(it - mChildren.begin());
}

Ptr<Node> Node::releaseChild(int index) noexcept
{
    TRJ_ASSERT(index >= 0 && index < (int) mChildren.size(), "Invalid child node index");

    Node* child = const_cast<Node*>(mChildren[index]);
    child->mParent = nullptr;
    mChildren.erase(mChildren.begin() + index);
    return Ptr<Node>(child);
}

Ptr<Node> Node::releaseChild(Node& child) noexcept
{
    auto it = std::find(mChildren.begin(), mChildren.end(), &child);

    TRJ_ASSERT(it != mChildren.end(), "Child not found");

    return releaseChild(it - mChildren.begin());
}

void Node::clearChildren() noexcept
{
    while(! mChildren.empty())
    {
        delete mChildren.back();
        mChildren.pop_back();
    }
}

std::vector<Ptr<Node>> Node::releaseChildren()
{
    std::vector<Ptr<Node>> releasedChildren;

    for(const Node* constChild : mChildren)
    {
        Node* child = const_cast<Node*>(constChild);
        child->mParent = nullptr;
        releasedChildren.push_back(Ptr<Node>(child));
    }

    mChildren.clear();
    return releasedChildren;
}

}
