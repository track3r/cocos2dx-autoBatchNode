//
//  IsomentrySprite.h
//  HelloCpp
//
//  Created by Alexandr Dem'yanenko on 7/17/13.
//
//

#ifndef __HelloCpp__IsomentrySprite__
#define __HelloCpp__IsomentrySprite__

#include "cocos2d.h"

class IsometryNode;

class IsometrySprite : public cocos2d::CCSprite
{
public:
    void setDirty(bool bDirty) override;
    void updateTransform(void) override;
    void addChild(CCNode* child, int zOrder, int tag) override;
    
    static IsometrySprite* createWithSpriteFrame(cocos2d::CCSpriteFrame *pSpriteFrame);
    static IsometrySprite* createWithSpriteFrameName(const char *pszSpriteFrameName);
    
    void SetIsometryNode(IsometryNode* node);
    void SetIsometryId(size_t id)
    {
        m_isometryId = id;
    }
    
    virtual void setScaleX(float fScaleX);
    virtual void setScaleY(float fScaleY);
    virtual void setPosition(const cocos2d::CCPoint& pos);
    virtual void setRotation(float fRotation);
    virtual void setRotationX(float fRotationX);
    virtual void setRotationY(float fRotationY);
    virtual void setSkewX(float sx);
    virtual void setSkewY(float sy);
    virtual void setScale(float fScale);
    virtual void setVertexZ(float fVertexZ);
    virtual void setAnchorPoint(const cocos2d::CCPoint& anchor);
    
    virtual void setDisplayFrame(cocos2d::CCSpriteFrame *pNewFrame);
    
private:
    IsometryNode* m_isometryNode = nullptr;
    size_t m_isometryId = -1;
};

#endif /* defined(__HelloCpp__IsomentrySprite__) */
