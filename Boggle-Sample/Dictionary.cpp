/*
 * Dictionary.cpp 
 *
 *   Implementation of the dictionary class.
 *
 *   version 2013-04-22 fixed bug with isWord on empty trie (10q MB+SC)
 *   version 2013-04-02
 */

#include <iostream>
#include <cctype>
#include "Dictionary.h"
using namespace std;

// ------------------------------ Node Functions -----------------------

Node::Node()
{
	for(int i=0; i<N_LETTS; i++)
		links[i] = NULL;
	is_word = false;
}

Node* Node::get_link(int pos)
{
	if ( pos >= 0 && pos <N_LETTS )
		return links[pos];
	return NULL;
}
bool Node::set_link(int pos, Node* p)
{
	if ( pos >= 0 && pos <N_LETTS ){
		links[pos] = p;
		return true;
	}
	return false;
}
bool Node::get_is_word()
{
	return is_word;
}
void Node::set_is_word(bool val)
{
	is_word = val;
}

// ------------------------------ Dict Functions -----------------------

static int char_to_int(char);		// a declaration

//
// An empty Dictionary has no root node, just a NULL ptr
//
Dictionary::Dictionary()
{
	root = NULL;
}

static void del_tree(Node*);
//
// destructor for a dictionary
//  recursively delete all nodes 
//
Dictionary::~Dictionary()
{
	del_tree(root);
}

// the actual destructor
// this is not a class function, it is just a private helper thing
// note: 'static' makes a function private to the source file
// 
static void del_tree(Node* r)
{
	if ( r != NULL )
	{
		for (int i = 0; i<N_LETTS; i++)
			del_tree( r->get_link(i) );
		delete r;
	}
}
//
// insert a string in the trie 
//   args: string
//   rets: true if did it, false if no mem
//   uses: isalpha, to_lower to map chars
//
bool Dictionary::insert(string s)
{
	Node	*iter     = root;
	Node	*next;
	int	next_level;

	if ( root == NULL ){				// empty tree?
		root = new Node;			// y: make node
		if ( root == NULL )			// no mem?
			return false;				// y: get out
		iter = root;				// start at root
	}
	// traverse the tree iteratively. Use chars in s to chose the path
	int slen = s.length();
	for( int pos = 0 ; pos < slen ; pos++ )
	{
		if ( !isalpha(s[pos]) )			// skip non-alphas
			continue;
		next_level = char_to_int(s[pos]);	// get array index 
		next = iter->get_link(next_level);	// from that get ptr
		if ( next == NULL ){			// if no node
			next = new Node;		// make one
			if ( next == NULL )		// no mem?
				return false;			// y: scram
			iter->set_link(next_level, next);	// n: store 
		}
		iter = next;				// advance level
	}
	iter->set_is_word(true);			// at end. mark word
	return true;					// it worked!
}

//
// tree_walk:
//   args: root of tree to walk, string to use as path
//   rets: ptr to Node we got to, or NULL if hit a wall
//   does: walk the tree using the chars as steps
//
static Node *tree_walk(Node*r, string s)
{
	int	slen;
	int	pos;
	Node	*iter = r;

	if ( iter != NULL ){
		slen = s.length();
		for( pos = 0 ; pos < slen ; pos++ )
		{
			if ( isalpha(s[pos]) )
			{
				iter = iter->get_link( char_to_int(s[pos]) );
				if ( iter == NULL )
					return NULL;
			}
		}
	}
	return iter;
}

//
// isWord(s)
//   return true if s is in dictionary
//
bool Dictionary::isWord(string s)
{
	Node	*end = tree_walk(root, s);

	return ( end != NULL && end->get_is_word() );
}

//
// isPrefix(s)
//   return true if s is a prefix in dictionary
//
bool Dictionary::isPrefix(string s)
{
	Node	*end = tree_walk(root, s);

	return ( end != NULL );
}

//
// char_to_int
//   args: a char
//   rets: the values 0..25 for a..z
//   note: assumes arg is an alpha char, 
//         otherwise returns 0
//
static int char_to_int(char c)
{
	if ( isalpha(c) )
		return ( tolower(c) - 'a' );
	return 0;
}
