//
//  IsometryNode.cpp
//  HelloCpp
//
//  Created by Alexandr Dem'yanenko on 7/17/13.
//
//

#include "IsometryNode.h"
#include "support/TransformUtils.h"
#include "shaders/CCGLProgram.h"
#include "kazmath/GL/matrix.h"
#include "IsometrySprite.h"

USING_NS_CC;

void IsometryNode::Each(CCNode* node,const std::function<void(CCNode* node)>&  func)
{
    auto children = node->getChildren();
    if(children == nullptr)
    {
        return;
    }
    
    for(size_t i = 0; i < children->count(); i++)
    {
        auto cur = (CCNode*)children->objectAtIndex(i);
        func(cur);
        Each(cur, func);
    }
}

inline bool QuadOverlaps(const ccV3F_C4B_T2F_Quad& a, const ccV3F_C4B_T2F_Quad& b)
{
    //return true;
    
    const float aMinX = fminf(a.tl.vertices.x, a.tr.vertices.x);
    const float aMaxX = fmaxf(a.tl.vertices.x, a.tr.vertices.x);
    
    const float aMinY = fminf(a.tl.vertices.y, a.bl.vertices.y);
    const float aMaxY = fmaxf(a.tl.vertices.y, a.bl.vertices.y);

    
    const float bMinX = fminf(b.tl.vertices.x, b.tr.vertices.x);
    const float bMaxX = fmaxf(b.tl.vertices.x, b.tr.vertices.x);
    
    const float bMinY = fminf(b.tl.vertices.y, b.bl.vertices.y);
    const float bMaxY = fmaxf(b.tl.vertices.y, b.bl.vertices.y);
    
    return  !(aMaxX < bMinX ||
            bMaxX < aMinX ||
            aMaxY < bMinY ||
            bMaxY < aMinY);
}

void SetQuadColors(ccV3F_C4B_T2F_Quad& quad, ccColor4B color)
{
    quad.tl.colors = color;
    quad.br.colors = color;
    quad.tr.colors = color;
    quad.bl.colors = color;
}

void DebugQuad(const ccV3F_C4B_T2F_Quad& quad)
{
    CCPoint points[] = {CCPointMake(quad.tl.vertices.x, quad.tl.vertices.y), CCPointMake(quad.tr.vertices.x, quad.tr.vertices.y),
        CCPointMake(quad.br.vertices.x, quad.br.vertices.y), CCPointMake(quad.bl.vertices.x, quad.bl.vertices.y),};

    ccDrawColor4F(1, 0, 0, 1);
    ccDrawLine(points[0], points[1]);
    
    ccDrawColor4F(0, 1, 0, 1);
    ccDrawLine(points[1], points[2]);
    
    ccDrawColor4F(0, 0, 1, 1);
    ccDrawLine(points[2], points[3]);

    ccDrawColor4F(0, 1, 1, 1);
    ccDrawLine(points[3], points[0]);

    ccDrawColor4F(1, 1, 1, 1);
}

void IsometryNode::Process()
{
    m_entries.clear();
    
    int32_t currentQuadIndex = 0;
    auto func = [&](CCNode* node)
    {
        auto sprite = dynamic_cast<CCSprite*>(node);
        if(sprite == nullptr)
        {
            return;
        }
        
        if(!sprite->isVisible())
        {
            return;
        }
        
        sprite->updateTransform();
        
        auto texture = sprite->getTexture()->getName();
        auto quad = sprite->getQuad();
        
        m_entries.push_back(IsometryNode::Entry(texture, quad));
        currentQuadIndex++;
    };
    
    Each((CCNode*)this, func);
    
    size_t entriesLeft = m_entries.size();
    int beginIndex = 0;
    
    std::vector<bool> bitset(entriesLeft);
    
    size_t numBatches = 0;
    
    while (entriesLeft)
    {
        const bool debugOutput = false;
        
        if(debugOutput)
        {
            CCLOG("Batch %zi", numBatches);
        }

        m_buffer.Clear();
        bool consecutive = true;
        const auto texture = m_entries[beginIndex].texture;
        
        if(debugOutput)
        {
            CCLOG("begin index = %zi(%zi)", beginIndex, texture);
        }
        
        for(size_t i = beginIndex; i < m_entries.size(); i++)
        {
            auto& quad = m_entries[i].quad;
            
            if(bitset[i])
            {
                continue;
            }
            
            if(consecutive)
            {
                beginIndex = i;
            }
            
            consecutive = false;

            if(m_entries[i].texture != texture)
            {
                continue;
            }
            
            bool canSubmitQuad = true;
            
            for(int j = i - 1; j >= beginIndex; j--)
            {
                if(bitset[j])
                {
                    continue;
                }
                
                if(QuadOverlaps(quad, m_entries[j].quad))
                {
                    if(debugOutput)
                    {
                        CCLOG("Can't submit %zi(%zi) - itersects with %zi(%zi)", i, m_entries[i].texture, j, m_entries[j].texture);
                    }
                    
                    canSubmitQuad = false;
                    break;
                }
                else if(debugOutput)
                {
                    CCLOG("%zi(%zi) doesn't itersects with %zi(%zi)", i, m_entries[i].texture, j, m_entries[j].texture);
                }
            }
            
            if(canSubmitQuad)
            {
                
                if(debugOutput)
                {
                    CCLOG("Submiting %zi(%zi)", i, m_entries[i].texture);
                }
                
                m_buffer.AddQuad(quad);
                
                bitset[i] = true;
                if(i == beginIndex)
                {
                    beginIndex++;
                }
                
                //DebugQuad(quad);
                entriesLeft--;
                
            }
        }
        
        if(m_buffer.Quads())
        {
            if(debugOutput)
            {
                CCLOG("Drawing batch with %zi elements", m_buffer.Quads());
            }
            getShaderProgram()->use();
            m_buffer.DrawAll(texture);
            numBatches++;
        }
    }
    
    if(numBatches != m_numBatches)
    {
        m_numBatches = numBatches;
        CCLOG("IsometryNode has %zi batches", m_numBatches);
    }
}

IsometryNode::IsometryNode()
{
    m_buffer.SetupVao();
}

void IsometryNode::draw(void)
{
    CC_PROFILER_START("CCSpriteBatchNode - draw");
    
    // Optimization: Fast Dispatch
    /*if( m_pobTextureAtlas->getTotalQuads() == 0 )
    {
        return;
    }*/
    
    CC_NODE_DRAW_SETUP();
    
    arrayMakeObjectsPerformSelector(m_pChildren, updateTransform, CCSprite*);
    
    //ccGLBlendFunc( m_blendFunc.src, m_blendFunc.dst );
    
    //m_pobTextureAtlas->drawQuads();
    Process();
    
    CC_PROFILER_STOP("CCSpriteBatchNode - draw");
}

void IsometryNode::addChild(CCNode* child, int zOrder, int tag)
{
    auto isoSprite = dynamic_cast<IsometrySprite*>(child);
    CCAssert(isoSprite, "");
    
    isoSprite->SetIsometryNode(this);
    
    CCNode::addChild(child, zOrder, tag);
    isoSprite->setDirty(true);
}

void IsometryNode::visit()
{
    //setPosition(CCPointMake(0, 500));
    //CCNode::visit();
    //Stats();
    if(getShaderProgram() == nullptr)
    {
        setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
    }
    
    CC_PROFILER_START_CATEGORY(kCCProfilerCategoryBatchSprite, "CCSpriteBatchNode - visit");
    
    // CAREFUL:
    // This visit is almost identical to CocosNode#visit
    // with the exception that it doesn't call visit on it's children
    //
    // The alternative is to have a void CCSprite#visit, but
    // although this is less maintainable, is faster
    //
    if (! m_bVisible)
    {
        return;
    }
    
    kmGLPushMatrix();
    
    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
        transformAncestors();
    }
    
    sortAllChildren();
    transform();

    draw();
    
    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->afterDraw(this);
    }
    
    kmGLPopMatrix();
    setOrderOfArrival(0);
    
    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategoryBatchSprite, "CCSpriteBatchNode - visit");
}