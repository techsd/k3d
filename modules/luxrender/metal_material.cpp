// K-3D
// Copyright (c) 1995-2009, Timothy M. Shead
//
// Contact: tshead@k-3d.com
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/** \file
	\author Tim Shead <tshead@k-3d.com>
*/

#include "material.h"

#include <k3d-i18n-config.h>
#include <k3dsdk/color.h>
#include <k3dsdk/document_plugin_factory.h>
#include <k3dsdk/iomanip.h>
#include <k3dsdk/imaterial.h>
#include <k3dsdk/measurement.h>
#include <k3dsdk/node.h>

namespace module
{

namespace luxrender
{

/////////////////////////////////////////////////////////////////////////////
// material

class metal_material :
	public k3d::node ,
	public luxrender::material
{
	typedef k3d::node base;

public:
	metal_material(k3d::iplugin_factory& Factory, k3d::idocument& Document) :
		base(Factory, Document),
		m_name(init_owner(*this) + init_name("name") + init_label(_("Name")) + init_description(_("Metal name, or the path to an NK optical properties file.")) + init_value(k3d::string_t("copper")) + init_values(wrap_names())),
		m_u_roughness(init_owner(*this) + init_name("u_roughness") + init_label(_("U Roughness")) + init_description(_("Roughness of the surface in the u direction.")) + init_value(0.001)),
		m_v_roughness(init_owner(*this) + init_name("v_roughness") + init_label(_("V Roughness")) + init_description(_("Roughness of the surface in the v direction.")) + init_value(0.001))
	{
	}

	void setup(std::ostream& Stream)
	{
		Stream << k3d::standard_indent << "Texture \"a\" \"float\" \"constant\" \"float value\" [" << m_u_roughness.pipeline_value() << "]\n";
		Stream << k3d::standard_indent << "Texture \"b\" \"float\" \"constant\" \"float value\" [" << m_v_roughness.pipeline_value() << "]\n";
		Stream << k3d::standard_indent << "Material \"metal\" \"string name\" \"" << m_name.pipeline_value() << "\" \"texture uroughness\" \"a\" \"texture vroughness\" \"b\" \n";
	}

	static k3d::iplugin_factory& get_factory()
	{
		static k3d::document_plugin_factory<metal_material,
				k3d::interface_list<k3d::imaterial> > factory(
			k3d::uuid(0x7bb2bd65, 0x764ec9c7, 0xdd52c5a2, 0x8a7048e9),
			"LuxRenderMetalMaterial",
			_("LuxRender Metal Material"),
			"LuxRender Material",
			k3d::iplugin_factory::EXPERIMENTAL);

		return factory;
	}

private:
	k3d_data(k3d::string_t, immutable_name, change_signal, with_undo, local_storage, no_constraint, list_property, with_serialization) m_name;
	k3d_data(k3d::double_t, immutable_name, change_signal, with_undo, local_storage, no_constraint, writable_property, with_serialization) m_u_roughness;
	k3d_data(k3d::double_t, immutable_name, change_signal, with_undo, local_storage, no_constraint, writable_property, with_serialization) m_v_roughness;

	static const k3d::ilist_property<std::string>::values_t& wrap_names()
	{
		static k3d::ilist_property<std::string>::values_t values;
		if(values.empty())
		{
			values.push_back("amorphous carbon");
			values.push_back("silver");
			values.push_back("gold");
			values.push_back("copper");
			values.push_back("aluminum");
		}
		return values;
	}
};

k3d::iplugin_factory& metal_material_factory()
{
	return metal_material::get_factory();
}

} // namespace luxrender

} // namespace module

