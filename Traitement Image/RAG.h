#ifndef _RAG_
#define _RAG_

#include <list>

class RAG
{
	public :
		RAG( int x, int y, int h, int w );
		~RAG( void );

		void add( RAG * r );
		void del( RAG * r );

		void verify( void );
		bool collide( RAG * r );

		int x, y, h, w;

		std::list<RAG*> f;
};

#endif
