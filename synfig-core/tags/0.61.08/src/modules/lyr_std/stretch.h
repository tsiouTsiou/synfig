/* === S Y N F I G ========================================================= */
/*!	\file stretch.h
**	\brief Header file for implementation of the "Stretch" layer
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

/* === S T A R T =========================================================== */

#ifndef __SYNFIG_LAYER_STRETCH_H
#define __SYNFIG_LAYER_STRETCH_H

/* === H E A D E R S ======================================================= */

#include <synfig/layer.h>
#include <synfig/vector.h>

/* === M A C R O S ========================================================= */

/* === T Y P E D E F S ===================================================== */

/* === C L A S S E S & S T R U C T S ======================================= */

class Stretch_Trans;
namespace synfig {

class Layer_Stretch : public Layer
{
	SYNFIG_LAYER_MODULE_EXT
	friend class ::Stretch_Trans;

private:

	Vector amount;
	Point center;

public:

	Layer_Stretch();

	virtual bool set_param(const String & param, const synfig::ValueBase &value);

	virtual ValueBase get_param(const String & param)const;

	virtual Color get_color(Context context, const Point &pos)const;

	virtual bool accelerated_render(Context context,Surface *surface,int quality, const RendDesc &renddesc, ProgressCallback *cb)const;
	synfig::Layer::Handle hit_check(synfig::Context context, const synfig::Point &point)const;

	virtual Vocab get_param_vocab()const;
	virtual etl::handle<synfig::Transform> get_transform()const;
	virtual synfig::Rect get_full_bounding_rect(Context context)const;
}; // END of class Layer_Stretch

}; // END of namespace synfig

/* === E N D =============================================================== */

#endif