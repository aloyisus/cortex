//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2013, Image Engine Design Inc. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//     * Neither the name of Image Engine Design nor the names of any
//       other contributors to this software may be used to endorse or
//       promote products derived from this software without specific prior
//       written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////

#include "boost/python.hpp"

#include "IECore/SceneInterface.h"
#include "IECorePython/RunTimeTypedBinding.h"
#include "IECorePython/IECoreBinding.h"

using namespace boost::python;
using namespace IECore;

namespace IECorePython
{

inline list arrayToList( std::vector<IndexedIO::EntryID> &ids )
{
	list result;
	for( SceneInterface::NameList::const_iterator it = ids.begin(); it!=ids.end(); it++ )
	{
		result.append( (*it).value() );
	}
	return result;
}

static list childNames( const SceneInterface &m )
{
	SceneInterface::NameList n;
	m.childNames( n );
	return arrayToList( n );
}

static list path( const SceneInterface &m )
{
	SceneInterface::Path p;
	m.path( p );
	return arrayToList( p );
}

static std::string pathAsString( const SceneInterface &m )
{
	SceneInterface::Path p;
	m.path( p );
	std::string str;
	SceneInterface::pathToString( p, str );
	return str;
}

static list readAttributeNames( const SceneInterface &m )
{
	SceneInterface::NameList a;
	m.readAttributeNames( a );
	return arrayToList( a );
}

static std::string pathToString( list l )
{
	SceneInterface::Path p;
	int listLen = IECorePython::len( l );
	for (int i = 0; i < listLen; i++ )
	{
		extract< std::string > ex( l[i] );
		if ( !ex.check() )
		{
			throw InvalidArgumentException( std::string( "Invalid path! Should be a list of strings!" ) );
		}
		p.push_back( ex() );	
	}
	std::string str;
	SceneInterface::pathToString( p, str );
	return str;
}

static list stringToPath( std::string str )
{
	SceneInterface::Path p;
	SceneInterface::stringToPath( str, p );
	return arrayToList( p );
}

static list supportedExtensions( IndexedIO::OpenMode modes )
{
	std::vector<std::string> e = SceneInterface::supportedExtensions( modes );
	list result;
	for( unsigned int i=0; i<e.size(); i++ )
	{
		result.append( e[i] );
	}
	return result;
}

void bindSceneInterface()
{
	SceneInterfacePtr (SceneInterface::*nonConstChild)(const SceneInterface::Name &, SceneInterface::MissingBehaviour) = &SceneInterface::child;
	SceneInterfacePtr (SceneInterface::*nonConstScene)(const SceneInterface::Path &, SceneInterface::MissingBehaviour) = &SceneInterface::scene;

	// make the SceneInterface class first
	IECorePython::RunTimeTypedClass<SceneInterface> sceneInterfaceClass;
	
	{
		// then define all the nested types
		scope s( sceneInterfaceClass );

		enum_< SceneInterface::MissingBehaviour > ("MissingBehaviour")
			.value("ThrowIfMissing", SceneInterface::ThrowIfMissing)
			.value("NullIfMissing", SceneInterface::NullIfMissing)
			.value("CreateIfMissing", SceneInterface::CreateIfMissing)
			.export_values()
		;
	}

	// now we've defined the nested types, we're able to define the methods for
	// the IndexedIO class itself (we need the definitions for the nested types 
	// to exist for defining default values).
	
	sceneInterfaceClass.def( "path", path )
		.def( "pathAsString", pathAsString )
		.def( "name", &SceneInterface::name, return_value_policy<copy_const_reference>() )
		.def( "readBound", &SceneInterface::readBound )
		.def( "writeBound", &SceneInterface::writeBound )
		.def( "readTransform", &SceneInterface::readTransform )
		.def( "readTransformAsMatrix", &SceneInterface::readTransformAsMatrix )
		.def( "writeTransform", &SceneInterface::writeTransform )
		.def( "hasAttribute", &SceneInterface::hasAttribute )
		.def( "readAttributeNames", readAttributeNames )
		.def( "readAttribute", &SceneInterface::readAttribute )
		.def( "writeAttribute", &SceneInterface::writeAttribute )
		.def( "readObject", &SceneInterface::readObject )
		.def( "writeObject", &SceneInterface::writeObject )
		.def( "hasObject", &SceneInterface::hasObject )
		.def( "hasChild", &SceneInterface::hasChild )
		.def( "childNames", &childNames )
		.def( "child", nonConstChild, ( arg( "name" ), arg( "missingBehaviour" ) = SceneInterface::ThrowIfMissing ) )
		.def( "createChild", &SceneInterface::createChild )
		.def( "scene", nonConstScene, ( arg( "path" ), arg( "missingBehaviour" ) = SceneInterface::ThrowIfMissing ) )

		.def( "pathToString", pathToString ).staticmethod("pathToString")
		.def( "stringToPath", stringToPath ).staticmethod("stringToPath")
		.def( "create", SceneInterface::create ).staticmethod( "create" )
		.def( "supportedExtensions", supportedExtensions, ( arg("modes") = IndexedIO::Read|IndexedIO::Write|IndexedIO::Append ) ).staticmethod( "supportedExtensions" )
	;
}

} // namespace IECorePython