//
//  IsomentrySprite.cpp
//  HelloCpp
//
//  Created by Alexandr Dem'yanenko on 7/17/13.
//
//

#include "IsometrySprite.h"
#include "ccTypes.h"
#include "IsometryNode.h"

USING_NS_CC;

#if CC_SPRITEBATCHNODE_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL(__ARGS__) (ceil(__ARGS__))
#endif

void IsometrySprite::setDirty(bool bDirty)
{
    CCSprite::setDirty(bDirty);

}

void IsometrySprite::updateTransform(void)
{
    //super hack
    m_pobBatchNode = (CCSpriteBatchNode*)m_isometryNode;
    m_transformToBatch = CCAffineTransformIdentity;
    CCSprite::updateTransform();
    m_pobBatchNode = nullptr;
}

void IsometrySprite::SetIsometryNode(IsometryNode* node)
{
    auto func = [&](CCNode* node)
    {
        auto isoSprite = dynamic_cast<IsometrySprite*>(node);
        CCAssert(isoSprite, "");
        isoSprite->m_isometryNode = m_isometryNode;
    };
    
    m_isometryNode = node;
    IsometryNode::Each((CCNode*)this, func);
    setDirtyRecursively(true);
}

void IsometrySprite::addChild(CCNode* child, int zOrder, int tag)
{
    auto isoSprite = dynamic_cast<IsometrySprite*>(child);
    CCAssert(isoSprite, "");
    
    isoSprite->SetIsometryNode(m_isometryNode);
    CCSprite::addChild(child, zOrder, tag);
    isoSprite->setDirtyRecursively(true);
}

IsometrySprite* IsometrySprite::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
    IsometrySprite *pobSprite = new IsometrySprite();
    if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

IsometrySprite* IsometrySprite::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
    CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
    
    return createWithSpriteFrame(pFrame);
}

void IsometrySprite::setPosition(const CCPoint& pos)
{
    CCNode::setPosition(pos);
    setDirtyRecursively(true);
}

void IsometrySprite::setRotation(float fRotation)
{
    CCNode::setRotation(fRotation);
    setDirtyRecursively(true);
}

void IsometrySprite::setRotationX(float fRotationX)
{
    CCNode::setRotationX(fRotationX);
    setDirtyRecursively(true);
}

void IsometrySprite::setRotationY(float fRotationY)
{
    CCNode::setRotationY(fRotationY);
    setDirtyRecursively(true);
}

void IsometrySprite::setSkewX(float sx)
{
    CCNode::setSkewX(sx);
    setDirtyRecursively(true);
}

void IsometrySprite::setSkewY(float sy)
{
    CCNode::setSkewY(sy);
    setDirtyRecursively(true);
}

void IsometrySprite::setScaleX(float fScaleX)
{
    CCNode::setScaleX(fScaleX);
    setDirtyRecursively(true);
}

void IsometrySprite::setScaleY(float fScaleY)
{
    CCNode::setScaleY(fScaleY);
    setDirtyRecursively(true);
}

void IsometrySprite::setScale(float fScale)
{
    CCNode::setScale(fScale);
    setDirtyRecursively(true);
}

void IsometrySprite::setVertexZ(float fVertexZ)
{
    CCNode::setVertexZ(fVertexZ);
    setDirtyRecursively(true);
}

void IsometrySprite::setAnchorPoint(const CCPoint& anchor)
{
    CCNode::setAnchorPoint(anchor);
    setDirtyRecursively(true);
}

void IsometrySprite::setDisplayFrame(cocos2d::CCSpriteFrame *pNewFrame)
{
    CCSprite::setDisplayFrame(pNewFrame);
    auto parent = getParent();
    if(parent != m_isometryNode)
    {
        ((IsometrySprite*)parent)->setDirty(true);
    }
    setDirtyRecursively(true);
}
