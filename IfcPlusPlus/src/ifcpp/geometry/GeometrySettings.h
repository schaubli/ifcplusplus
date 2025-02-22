/* -*-c++-*- IfcQuery www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#define _USE_MATH_DEFINES 
#include <cmath>
#include <ifcpp/model/OpenMPIncludes.h>
#include <functional>
#include <ifcpp/IFC4/include/IfcFeatureElementSubtraction.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#define GEOM_TOLERANCE  0.0000001
#define HALF_SPACE_BOX_SIZE 100

//#define ROUND_IFC_COORDINATES
#ifdef ROUND_IFC_COORDINATES
	#define ROUND_IFC_COORDINATES_UP 100000.0
	#define ROUND_IFC_COORDINATES_DOWN 0.00001
#endif

//\brief Central class to hold settings that influence geometry processing.

class GeometrySettings
{
public:
	bool m_retriangulate_mesh_before_csg = true;

protected:
	int	m_num_vertices_per_circle = 14;
	int m_num_vertices_per_circle_default = 14;
	int m_min_num_vertices_per_arc = 6;
	int m_num_vertices_per_control_point = 4;
	int m_num_vertices_per_control_point_default = 4;
	bool m_show_text_literals = false;
	bool m_ignore_profile_radius = false;
	bool m_handle_styled_items = true;
	bool m_handle_layer_assignments = true;
	bool m_render_crease_edges = true;
	bool m_render_bounding_box = false;
	double m_crease_edges_max_delta_angle = M_PI*0.05;
	double m_crease_edges_line_width = 1.5;
	double m_colinear_faces_max_delta_angle = M_PI*0.02;
	double m_min_triangle_area = 0.005*0.005;

	std::function<int(double)> m_num_vertices_per_circle_given_radius = [&](double radius) {
		if (radius > 0.5) return int(m_num_vertices_per_circle*1.5);
		return m_num_vertices_per_circle;
	};
	std::function<bool(const shared_ptr<IfcObjectDefinition>&)> m_render_object_filter =
		[](const shared_ptr<IfcObjectDefinition>& ifc_object) {
		return dynamic_pointer_cast<IfcFeatureElementSubtraction>(ifc_object) == nullptr;
	};

public:
	GeometrySettings()
	{
	}

	// Number of discretization points per circle
	int getNumVerticesPerCircleWithRadius(double radius) { return m_num_vertices_per_circle_given_radius(radius); }
	int getNumVerticesPerCircle() { return m_num_vertices_per_circle; }
	void setNumVerticesPerCircle( int num )
	{
		m_num_vertices_per_circle = num;
	}
	void resetNumVerticesPerCircle()
	{
		m_num_vertices_per_circle = m_num_vertices_per_circle_default;
	}
    void setNumVerticesPerCircleGivenRadius(std::function<int(double)> f) {
        m_num_vertices_per_circle_given_radius = f;
    }

	// Minimum number of discretization points per arc
	int getMinNumVerticesPerArc() { return m_min_num_vertices_per_arc; }
	void resetMinNumVerticesPerArc()
	{
		m_min_num_vertices_per_arc = 6;
	}
	void setMinNumVerticesPerArc( int num )
	{
		m_min_num_vertices_per_arc = num;
	}

	int getNumVerticesPerControlPoint() { return m_num_vertices_per_control_point; }
	void setNumVerticesPerControlPoint( int num )
	{
		m_num_vertices_per_control_point = num;
	}
	void resetNumVerticesPerControlPoint()
	{
		m_num_vertices_per_control_point = m_num_vertices_per_control_point_default;
	}

	void setHandleLayerAssignments( bool handle ) { m_handle_layer_assignments = handle; }
	bool handleLayerAssignments() { return m_handle_layer_assignments; }
	
	void setHandleStyledItems( bool handle ) { m_handle_styled_items = handle; }
	bool handleStyledItems() { return m_handle_styled_items; }

	bool isShowTextLiterals() { return m_show_text_literals; }
	bool isIgnoreProfileRadius() { return m_ignore_profile_radius; }
	void setIgnoreProfileRadius( bool ignore_radius )
	{
		m_ignore_profile_radius = ignore_radius;
	}

	/**\brief If angle between two faces is smaller than max_delta, the normals will be added and normalized.
	Set to negative value to deactivate intermediate normals */
	void setCoplanarFacesMaxDeltaAngle( double max_delta )
	{
		m_colinear_faces_max_delta_angle = max_delta;
	}
	double getCoplanarFacesMaxDeltaAngle() { return m_colinear_faces_max_delta_angle; }

	/**\brief setMinTriangleArea: if a triangle is smaller than this value, it is still in the carve meshset, but skipped for rendering.
	That reduces the number of triangles on the GPU, not visible unless you zoom in to a very small area */
	void setMinTriangleArea(double min_area)
	{
		m_min_triangle_area = min_area;
	}
	double getMinTriangleArea() { return m_min_triangle_area; }
	

	/**\brief If angle between two faces is smaller than max_delta, the normals will be added and normalized. */
	void setCreaseEdgesMaxDeltaAngle( double max_delta )
	{
		m_crease_edges_max_delta_angle = max_delta;
	}
	double getCreaseEdgesMaxDeltaAngle() { return m_crease_edges_max_delta_angle; }

	/**\brief Set line width of crease edges. If <= 0, crease edges are not rendered */
	void setCreaseEdgesLineWidth(double w)
	{
		m_crease_edges_line_width = w;
	}
	double getCreaseEdgesLineWidth() { return m_crease_edges_line_width; }

	/**\brief Render crease edges */
	bool getRenderCreaseEdges() { return m_render_crease_edges; }
	void setRenderCreaseEdges( bool render_crease_edges ) { m_render_crease_edges = render_crease_edges; }

	/**\brief Render bounding box for each object */
	bool getRenderBoundingBoxes() { return m_render_bounding_box; }
	void setRenderBoundingBoxes( bool render_bbox ) { m_render_bounding_box = render_bbox; }

    /**\brief Render filter decides if a IfcObjectDefinition should be rendered. 
	  The default filter will render all objects except objects based on IfcFeatureElementSubtraction.*/
	std::function<bool(const shared_ptr<IfcObjectDefinition>&)>getRenderObjectFilter() const {
		return m_render_object_filter;
	}
	void setRenderObjectFilter(std::function<bool(const shared_ptr<IfcObjectDefinition>&)> render_filter) {
		m_render_object_filter = std::move(render_filter);
	}
};
