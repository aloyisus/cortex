//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008-2009, Image Engine Design Inc. All rights reserved.
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

#include "IECore/UniformRandomPointDistributionOp.h"
#include "IECore/Parameter.h"
#include "IECore/Object.h"
#include "IECore/CompoundObject.h"
#include "IECore/bindings/RunTimeTypedBinding.h"
#include "IECore/bindings/Wrapper.h"

using namespace boost;
using namespace boost::python;

namespace IECore
{


class UniformRandomPointDistributionOpWrap : 
	public UniformRandomPointDistributionOp, public Wrapper<UniformRandomPointDistributionOp>
{
	public :

		IE_CORE_DECLAREMEMBERPTR( UniformRandomPointDistributionOpWrap );
				
		UniformRandomPointDistributionOpWrap( PyObject *self ) : UniformRandomPointDistributionOp(), Wrapper<UniformRandomPointDistributionOp>( self, this )
		{
		}
		
		UniformRandomPointDistributionOpWrap( PyObject *self, const std::string &name, const std::string &description ) : UniformRandomPointDistributionOp( name, description ), Wrapper<UniformRandomPointDistributionOp>( self, this )
		{
		}
		
		virtual ~UniformRandomPointDistributionOpWrap()
		{
		}
		
		virtual float density( ConstMeshPrimitivePtr mesh, const Imath::V3f &point, const Imath::V2f &uv ) const
		{
			override o = this->get_override( "density" );
			if( o )
			{
				return o( mesh, point, uv );
			}
			else
			{
				return UniformRandomPointDistributionOp::density( mesh, point, uv );
			}
		};

};

void bindUniformRandomPointDistributionOp()
{
	RunTimeTypedClass<UniformRandomPointDistributionOp, UniformRandomPointDistributionOpWrap::Ptr>()
		.def( init<>() )
		.def( init< const std::string &, const std::string &>() )
		.def( "density", &UniformRandomPointDistributionOpWrap::density )
	;
}

} // namespace IECore
