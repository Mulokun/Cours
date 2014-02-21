#include "RAG.h"

RAG::RAG( int x, int y, int h, int w ) : x(x), y(y), h(h), w(w) {}
RAG::~RAG( void ) {}


void RAG::add( RAG * r )
{
	this->f.remove( r );
	this->f.push_back( r );

	r->f.remove(this);
	r->f.push_back( this );
}

void RAG::del( RAG * r )
{
	this->f.remove( r );
	r->f.remove( this );
}

void RAG::verify( void )
{
	for(std::list<RAG*>::iterator i = f.begin(); i != f.end(); ++i) {
		if(! collide( *i )) {
			RAG * r = *i;
			i--;
			f.remove( r );
		}
	}
}

bool RAG::collide( RAG * r )
{
	return (
		(x >= r->x && x <= r->x + r->h && y >= r->y && y <= r->y + r->w)
		||
		(x >= r->x && x <= r->x + r->h && y + w >= r->y && y + w <= r->y + r->w)
		||
		(x + h >= r->x && x + h <= r->x + r->h && y + w >= r->y && y + w<= r->y + r->w)
		||
		(x + h >= r->x && x + h <= r->x + r->h && y >= r->y && y <= r->y + r->w)
	);
}


