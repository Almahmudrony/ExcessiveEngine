#pragma once

#include "IBuffer.h"
#include "BufferGL.h"

class VertexBufferGL : public BufferGL, public IVertexBuffer
{
  public:
    void destroy()
    {
      BufferGL::destroy();
    }

	void update(void* data, unsigned size, unsigned offset) override
    {
      /*void* ptr = glMapBufferRange( GL_ARRAY_BUFFER, offset, size, GL_MAP_WRITE_BIT );
      memcpy( ptr, data, size );
      glUnmapBuffer( GL_ARRAY_BUFFER );*/

		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }

	void getSubData(void* data, unsigned size, unsigned offset) override
    {
		BufferGL::getSubData(data, size, offset);
    }

	rBuffer getDesc()
    {
		return BufferGL::getDesc();
    }
};