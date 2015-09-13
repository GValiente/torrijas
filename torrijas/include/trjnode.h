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

#ifndef TRJ_NODE_H
#define TRJ_NODE_H

#include "trjstring.h"
#include "trjshapegroup.h"
#include "trjaction.h"

namespace trj
{

class RenderContext;
class Application;

class Node
{
    friend class Application;

protected:
    Point mPosition;
    Color mBlendColor;
    Rect mScissorRect;
    Rect mBoundingBox;
    Rect mFinalBoundingBox;
    String mTag;
    Node* mParent = nullptr;
    void* mRenderCache = nullptr;
    float mRotationAngle = 0;
    float mSkewXAngle = 0;
    float mSkewYAngle = 0;
    float mScaleX = 1;
    float mScaleY = 1;
    float mOpacity = 1;
    float mBlendColorFactor = 0;
    float mActionsSpeed = 1;
    float mFinalScaleX = 1;
    float mFinalScaleY = 1;
    bool mVisible = true;
    bool mHidden = false;
    bool mMoveWithScreenWidth = false;
    bool mMoveWithScreenHeight = false;
    bool mScaleWithScreenAspectRatio = false;
    bool mFlipX = false;
    bool mFlipY = false;
    bool mActionsRunning = false;
    bool mActionsPaused = false;
    bool mRenderOffScreen = false;
    bool mInvalidateBoundingBox = false;
    bool mInvalidateRenderCache = false;
    bool mInvalidateTransform = true;
    bool mInvalidateHidden = true;
    bool mIsOnScreen = false;

    std::vector<ShapeGroup> mShapeGroups;
    std::vector<Ptr<Action>> mActions;
    std::vector<const Node*> mChildren;
    std::array<float, 6> mTransform;
    std::array<float, 6> mFinalTransform;

    Node() noexcept
    {
    }

    Node(const Node& other);

    virtual Rect generateBoundingBox();

    virtual void updateItself(float elapsedTime);

    virtual bool renderCacheAvailable(const RenderContext& renderContext) const;

    virtual void renderItself(RenderContext& renderContext);

    void addActionImpl(Ptr<Action>&& action);

    void addChildImpl(Ptr<Node>&& child);

    void insertChildImpl(int index, Ptr<Node>&& child);

    void setChildImpl(int index, Ptr<Node>&& child) noexcept;

    void update(float elapsedTime, float actionsElapsedTime, bool actionsPaused);

    void render(RenderContext& renderContext);

    void invalidateBoundingBox() noexcept
    {
        mInvalidateBoundingBox = true;
        invalidateRenderCache();
    }

    void invalidateRenderCache() noexcept
    {
        mInvalidateRenderCache = true;
    }

    void invalidateTransform() noexcept
    {
        mInvalidateTransform = true;
    }

    void invalidateHidden() noexcept
    {
        mInvalidateHidden = true;
    }

    void updateTransform(RenderContext& renderContext);

public:
    static Ptr<Node> create()
    {
        return Ptr<Node>(new Node());
    }

    static Node& getRootNode() noexcept;

    Node& operator=(const Node& other) = delete;

    virtual ~Node();

    virtual Ptr<Node> getClone() const
    {
        return Ptr<Node>(new Node(*this));
    }

    const Point& getPosition() const noexcept
    {
        return mPosition;
    }

    void setPosition(const Point& position) noexcept
    {
        mPosition = position;
        invalidateTransform();
    }

    void setPosition(float x, float y) noexcept
    {
        setPosition(Point(x, y));
    }

    void setPositionX(float x) noexcept
    {
        setPosition(x, mPosition.getY());
    }

    void setPositionY(float y) noexcept
    {
        setPosition(mPosition.getX(), y);
    }

    const Rect& getBoundingBox();

    float getRotationAngle() const noexcept
    {
        return mRotationAngle;
    }

    void setRotationAngle(float angle) noexcept;

    float getSkewXAngle() const noexcept
    {
        return mSkewXAngle;
    }

    void setSkewXAngle(float angle) noexcept
    {
        mSkewXAngle = angle;
        invalidateTransform();
    }

    float getSkewYAngle() const noexcept
    {
        return mSkewYAngle;
    }

    void setSkewYAngle(float angle) noexcept
    {
        mSkewYAngle = angle;
        invalidateTransform();
    }

    void setSkewAngle(float xAngle, float yAngle) noexcept
    {
        setSkewXAngle(xAngle);
        setSkewYAngle(yAngle);
    }

    float getScaleX() const noexcept
    {
        return mScaleX;
    }

    float getScaleY() const noexcept
    {
        return mScaleY;
    }

    void setScaleX(float scale) noexcept;

    void setScaleY(float scale) noexcept;

    void setScale(float scaleX, float scaleY) noexcept
    {
        setScaleX(scaleX);
        setScaleY(scaleY);
    }

    void setScale(float scale) noexcept
    {
        setScale(scale, scale);
    }

    bool scaleWithScreenAspectRatio() const noexcept
    {
        return mScaleWithScreenAspectRatio;
    }

    void setScaleWithScreenAspectRatio(bool scale) noexcept
    {
        mScaleWithScreenAspectRatio = scale;
        invalidateTransform();
    }

    bool getFlipX() const noexcept
    {
        return mFlipX;
    }

    void setFlipX(bool flipX) noexcept
    {
        mFlipX = flipX;
        invalidateTransform();
    }

    bool getFlipY() const noexcept
    {
        return mFlipY;
    }

    void setFlipY(bool flipY) noexcept
    {
        mFlipY = flipY;
        invalidateTransform();
    }

    float getOpacity() const noexcept
    {
        return mOpacity;
    }

    void setOpacity(float opacity) noexcept;

    const Color& getBlendColor() const noexcept
    {
        return mBlendColor;
    }

    float getBlendColorFactor() const noexcept
    {
        return mBlendColorFactor;
    }

    void setBlendColor(const Color& blendColor, float blendFactor) noexcept;

    void setBlendColor(float red, float green, float blue, float blendFactor) noexcept
    {
        setBlendColor(Color(red, green, blue), blendFactor);
    }

    void clearBlendColor() noexcept
    {
        setBlendColor(Color(), 0);
    }

    const Rect& getScissorRect() const noexcept
    {
        return mScissorRect;
    }

    void setScissorRect(const Rect& scissorRect) noexcept
    {
        mScissorRect = scissorRect;
    }

    void setScissorRect(float positionX, float positionY, float width, float height) noexcept
    {
        mScissorRect = Rect(positionX, positionY, width, height);
    }

    const String& getTag() const noexcept
    {
        return mTag;
    }

    void setTag(String tag) noexcept
    {
        mTag = std::move(tag);
    }

    bool isVisible() const noexcept
    {
        return mVisible;
    }

    void setVisible(bool visible) noexcept
    {
        mVisible = visible;
        invalidateHidden();
    }

    bool renderOffScreen() const noexcept
    {
        return mRenderOffScreen;
    }

    void setRenderOffScreen(bool renderOffScreen) noexcept
    {
        mRenderOffScreen = renderOffScreen;
    }

    const Node* getParent() const noexcept
    {
        return mParent;
    }

    Node* getParent() noexcept
    {
        return mParent;
    }

    const std::vector<ShapeGroup>& getShapeGroups() const noexcept
    {
        return mShapeGroups;
    }

    void reserveShapeGroups(int numShapeGroups);

    void addShapeGroup(ShapeGroup shapeGroup)
    {
        mShapeGroups.push_back(std::move(shapeGroup));
        invalidateBoundingBox();
    }

    void insertShapeGroup(int index, ShapeGroup shapeGroup);

    void setShapeGroup(int index, ShapeGroup shapeGroup) noexcept;

    void setShapeGroups(std::vector<ShapeGroup> shapeGroups) noexcept
    {
        mShapeGroups = std::move(shapeGroups);
        invalidateBoundingBox();
    }

    void removeShapeGroup(int index) noexcept;

    ShapeGroup releaseShapeGroup(int index) noexcept;

    void clearShapeGroups() noexcept
    {
        mShapeGroups.clear();
        invalidateBoundingBox();
    }

    void resetShapeGroups() noexcept
    {
        std::vector<ShapeGroup>().swap(mShapeGroups);
        invalidateBoundingBox();
    }

    std::vector<ShapeGroup> releaseShapeGroups() noexcept;

    int getNumActiveActions() const noexcept
    {
        return mActions.size();
    }

    void reserveActions(int numActions);

    void setActions(std::vector<Ptr<Action>>&& actions) noexcept;

    template<class ActionClass>
    inline void addAction(Ptr<ActionClass>&& action)
    {
        addActionImpl(std::move(action));
    }

    void clearActions() noexcept;

    void resetActions() noexcept;

    float getActionsSpeed() const noexcept
    {
        return mActionsSpeed;
    }

    void setActionsSpeed(float speed) noexcept;

    bool areActionsPaused() const noexcept
    {
        return mActionsPaused;
    }

    void setActionsPaused(bool paused) noexcept;

    bool areActionsRunning() const noexcept
    {
        return mActionsRunning;
    }

    const std::vector<const Node*>& getChildren() const noexcept
    {
        return mChildren;
    }

    const Node& getChild(int index) const noexcept;

    Node& getChild(int index) noexcept;

    const Node* findChild(const String& tag) const noexcept;

    Node* findChild(const String& tag) noexcept;

    const Node* findChildRecursive(const String& tag) const noexcept;

    Node* findChildRecursive(const String& tag) noexcept;

    void reserveChildren(int numChildren);

    template<class NodeClass>
    inline NodeClass& addChild(Ptr<NodeClass>&& child)
    {
        NodeClass* childPtr = child.get();
        addChildImpl(std::move(child));
        return *childPtr;
    }

    Node& addNewChild()
    {
        return addChild(create());
    }

    template<class NodeClass>
    inline NodeClass& insertChild(int index, Ptr<NodeClass>&& child)
    {
        NodeClass* childPtr = child.get();
        insertChildImpl(index, std::move(child));
        return *childPtr;
    }

    Node& insertNewChild(int index)
    {
        return insertChild(index, create());
    }

    template<class NodeClass>
    inline NodeClass& setChild(int index, Ptr<NodeClass>&& child) noexcept
    {
        NodeClass* childPtr = child.get();
        setChildImpl(index, std::move(child));
        return *childPtr;
    }

    Node& setNewChild(int index)
    {
        return setChild(index, create());
    }

    void setChildren(std::vector<Ptr<Node>>&& children);

    void removeChild(int index) noexcept;

    void removeChild(Node& child) noexcept;

    Ptr<Node> releaseChild(int index) noexcept;

    Ptr<Node> releaseChild(Node& child) noexcept;

    void clearChildren() noexcept;

    void resetChildren() noexcept
    {
        clearChildren();
        std::vector<const Node*>().swap(mChildren);
    }

    std::vector<Ptr<Node>> releaseChildren();
};

}

#endif
