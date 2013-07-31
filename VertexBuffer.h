//
//  VertexBuffer.h
//  HelloCpp
//
//  Created by Alexandr Dem'yanenko on 7/18/13.
//
//

#ifndef __HelloCpp__VertexBuffer__
#define __HelloCpp__VertexBuffer__

#include "cocos2d.h"
#include "ccTypes.h"
#include <vector>

struct QuadIndices
{
    GLushort indices[6];
};

class VertexBuffer
{
public:
    void Draw(int start, int count, GLuint texture);
    void DrawAll(GLuint texture);
    void SetupVao();
    void AddQuad(const cocos2d::ccV3F_C4B_T2F_Quad& quad);
    void Clear();
    size_t Quads() const;

private:
    void MapBuffers();
    
private:
    std::vector<cocos2d::ccV3F_C4B_T2F_Quad> m_quads;
    std::vector<QuadIndices> m_indices;
    size_t m_usedIndices = 0;
    GLuint m_buffersNames[2] = {0}; //0: vertex  1: indices
    bool m_quadsDirty = true;
    bool m_indicesDirty = true;

};

#endif /* defined(__HelloCpp__VertexBuffer__) */
