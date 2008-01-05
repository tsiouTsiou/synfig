/* === S Y N F I G ========================================================= */
/*!	\file layer_duplicate.cpp
**	\brief Implementation of the "Duplicate" layer
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**	Copyright (c) 2007 Chris Moore
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**	\endlegal
*/
/* ========================================================================= */

/* === H E A D E R S ======================================================= */

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "string.h"
#include "layer_duplicate.h"
#include "time.h"
#include "context.h"
#include "paramdesc.h"
#include "renddesc.h"
#include "surface.h"
#include "value.h"
#include "valuenode.h"
#include "canvas.h"

#endif

/* === U S I N G =========================================================== */

using namespace synfig;
using namespace etl;
using namespace std;

/* === G L O B A L S ======================================================= */

SYNFIG_LAYER_INIT(Layer_Duplicate);
SYNFIG_LAYER_SET_NAME(Layer_Duplicate,"duplicate");
SYNFIG_LAYER_SET_LOCAL_NAME(Layer_Duplicate,N_("Duplicate"));
SYNFIG_LAYER_SET_CATEGORY(Layer_Duplicate,N_("Other"));
SYNFIG_LAYER_SET_VERSION(Layer_Duplicate,"0.1");
SYNFIG_LAYER_SET_CVS_ID(Layer_Duplicate,"$Id$");

/* === M E M B E R S ======================================================= */

Layer_Duplicate::Layer_Duplicate():
	Layer_Composite(1.0,Color::BLEND_COMPOSITE)
{
	LinkableValueNode* index_value_node = ValueNode_Duplicate::create(int(3));
	connect_dynamic_param("index", index_value_node);
}

bool
Layer_Duplicate::set_param(const String &param, const ValueBase &value)
{
	IMPORT(index);
	return Layer_Composite::set_param(param,value);
}

ValueBase
Layer_Duplicate::get_param(const String &param)const
{
 	EXPORT(index);

	EXPORT_NAME();
	EXPORT_VERSION();

	return Layer_Composite::get_param(param);
}

void
Layer_Duplicate::set_time(Context context, Time time)const
{
	context.set_time(time);
	time_cur=time;
}

void
Layer_Duplicate::set_time(Context context, Time time, const Point &pos)const
{
	context.set_time(time,pos);
	time_cur=time;
}

Color
Layer_Duplicate::get_color(Context context, const Point &pos)const
{
	return context.get_color(pos);
}

Layer::Vocab
Layer_Duplicate::get_param_vocab()const
{
	Layer::Vocab ret;
	//ret=Layer_Composite::get_param_vocab();

	ret.push_back(ParamDesc("index")
		.set_local_name(_("Index"))
		.set_description(_("Copy Index"))
	);

	return ret;
}

bool
Layer_Duplicate::accelerated_render(Context context,Surface *surface,int quality, const RendDesc &renddesc, ProgressCallback *cb)const
{
	if(quality == 10)
		return context.accelerated_render(surface,quality,renddesc,cb);

	if(context->empty())
	{
		surface->set_wh(renddesc.get_w(),renddesc.get_h());
		surface->clear();
		return true;
	}

	SuperCallback subimagecb;
	Surface tmp;
	int i = 0;

	const DynamicParamList &dpl = dynamic_param_list();
	DynamicParamList::const_iterator iter = dpl.find("index");
	if (iter == dpl.end())
		return context.accelerated_render(surface,quality,renddesc,cb);

	etl::rhandle<ValueNode> param(iter->second);
	handle<ValueNode_Duplicate>	duplicate_param(handle<ValueNode_Duplicate>::cast_dynamic(param));

	if (!duplicate_param)
		return context.accelerated_render(surface,quality,renddesc,cb);

	surface->set_wh(renddesc.get_w(),renddesc.get_h());
	surface->clear();

	Color::BlendMethod blend_method(get_blend_method());
	int steps = duplicate_param->count_steps(time_cur);

	duplicate_param->reset_index(time_cur);
	do
	{
		subimagecb=SuperCallback(cb,i*(5000/steps),(i+1)*(5000/steps),5000);
		// \todo can we force a re-evaluation of all the variables without changing the time twice?
		context.set_time(time_cur+1);
		context.set_time(time_cur);
		if(!context.accelerated_render(&tmp,quality,renddesc,&subimagecb)) return false;

		Surface::alpha_pen apen(surface->begin());
		apen.set_alpha(1.0);
		// \todo have a checkbox allowing use of 'behind' to reverse the order?
		apen.set_blend_method(blend_method);
		tmp.blit_to(apen);
		i++;
	} while (duplicate_param->step(time_cur));

	duplicate_param->reset_index(time_cur);

	return true;
}