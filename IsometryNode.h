//
//  IsometryNode.h
//  HelloCpp
//
//  Created by Alexandr Dem'yanenko on 7/17/13.
//
//

#ifndef __HelloCpp__IsometryNode__
#define __HelloCpp__IsometryNode__

#include "cocos2d.h"

#include "VertexBuffer.h"

class IsometrySprite;

class IsometryNode : public cocos2d::CCNode
{
public:
    struct Entry
    {
        Entry(size_t texture, const cocos2d::ccV3F_C4B_T2F_Quad& quad)
            :texture(texture)
            ,quad(quad)
        {
            
        }
        size_t texture;
        size_t id;
        cocos2d::ccV3F_C4B_T2F_Quad quad;
    };
    
    IsometryNode();
    
    void visit() override;
    void draw() override;
    void addChild(CCNode* child, int zOrder, int tag);
    
    static void Each(CCNode* node,const std::function<void(CCNode* node)>&  func);

private:
    void Process();

private:
    VertexBuffer m_buffer;
    typedef std::vector<IsometryNode::Entry> Entries;
    Entries m_entries;
    size_t m_numBatches = 0;
};


#endif /* defined(__HelloCpp__IsometryNode__) */
