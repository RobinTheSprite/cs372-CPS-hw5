//
// Created by Mark, Bryant and Jacob on 3/20/2019.
//

#include "compoundshape.hpp"

namespace cps {

using std::vector;
using std::stringstream;
using std::move;
using std::to_string;
using std::pair;

CompoundShape::CompoundShape(vector<Shape_ptr> shapes)
	: _shapes(move(shapes))
{}

void CompoundShape::pushShape(Shape_ptr shape)
{
	_shapes.push_back(move(shape));
}

size_t CompoundShape::get_numShapes() const {
	return _shapes.size();
}

CompoundShape::iterator CompoundShape::begin()
{
	return _shapes.begin();
}
CompoundShape::iterator CompoundShape::end()
{
	return _shapes.end();
}
CompoundShape::const_iterator CompoundShape::begin() const
{
	return _shapes.begin();
}
CompoundShape::const_iterator CompoundShape::end() const
{
	return _shapes.end();
}

std::stringstream CompoundShape::generate()
{
    stringstream postScriptFragment;
    auto relativeCurrentPoint{0.0};
    for (auto shape = begin(); shape != end(); ++shape)
    {
        if (shape != begin())
        {
           postScriptFragment << moveToNextShape(**shape, relativeCurrentPoint);
        }
        postScriptFragment << (*shape)->generate().str();
        if (shape + 1 != end())
        {
            moveToNextShape(**shape, relativeCurrentPoint);
        }
    }
    if (get_numShapes() > 1)
    {
       postScriptFragment << moveBackToOrigin(relativeCurrentPoint);
    }
    return postScriptFragment;
}

    LayeredShapes::LayeredShapes(std::vector<Shape_ptr> shapes)
	: CompoundShape(move(shapes))
{}

double LayeredShapes::get_height() const
{
    auto maxHeight{0.0};
    for (const auto & shape : *this)
    {
        if (shape->get_height() > maxHeight) {
            maxHeight = shape->get_height();
        }
    }
    return maxHeight;
}

double LayeredShapes::get_width() const
{
	auto maxWidth{0.0};
	for (const auto & shape : *this)
	{
		if (shape->get_width() > maxWidth) {
			maxWidth = shape->get_width();
		}
	}
	return maxWidth;
}

stringstream LayeredShapes::generate()
{
	stringstream postScriptFragment;
	for (auto & shape : *this)
	{
		postScriptFragment << shape->generate().str() << "\n";
	}
	return postScriptFragment;
}

HorizontalShapes::HorizontalShapes(std::vector<Shape_ptr> shapes)
	: CompoundShape(move(shapes))
{}

double HorizontalShapes::get_height() const
{
	auto maxHeight{0.0};
	for (const auto & shape : *this)
	{
		if (shape->get_height() > maxHeight) {
			maxHeight = shape->get_height();
		}
	}
	return maxHeight;
}

double HorizontalShapes::get_width() const
{
    auto totalWidth{0.0};
    for (const auto & shape : *this)
    {
        totalWidth += shape->get_width();
    }
    return totalWidth;
}

std::stringstream HorizontalShapes::generate()
{
	stringstream postScriptFragment;
	auto relativeCurrentPoint{0.0};
	for (auto shape = begin(); shape != end(); ++shape)
	{
	    if (shape != begin())
        {
            relativeCurrentPoint += (*shape)->get_width()/2;
			postScriptFragment << std::to_string((*shape)->get_width()/2) << " " << "0 translate\n";
        }
		postScriptFragment << (*shape)->generate().str();
		if (shape + 1 != end()) {
            relativeCurrentPoint += (*shape)->get_width()/2;
			postScriptFragment << std::to_string((*shape)->get_width()/2) << " " << "0 translate\n";
		}
	}
	if (get_numShapes() > 1) {
		postScriptFragment << to_string(-relativeCurrentPoint) << " 0 translate\n";
	}
	return postScriptFragment;
}

VerticalShapes::VerticalShapes(vector<Shape_ptr> shapes)
	: CompoundShape(move(shapes))
{}

double VerticalShapes::get_height() const
{
    auto totalHeight{0.0};
    for (const auto & shape : *this)
    {
        totalHeight += shape->get_width();
    }
    return totalHeight;
}

double VerticalShapes::get_width() const
{
    auto maxWidth{0.0};
    for (const auto & shape : *this)
    {
        if (shape->get_width() > maxWidth) {
            maxWidth = shape->get_width();
        }
    }
    return maxWidth;
}

stringstream VerticalShapes::generate()
{
	stringstream postScriptFragment;
	auto relativeCurrentPoint{0.0};
	for (auto shape = begin(); shape != end(); ++shape)
	{
	    if (shape != begin())
        {
            relativeCurrentPoint += (*shape)->get_height()/2;
			postScriptFragment << 0 << " " << to_string((*shape)->get_height()/2) << " translate\n";
        }
		postScriptFragment << (*shape)->generate().str();
		if (shape + 1 != end()) {
			relativeCurrentPoint += (*shape)->get_height()/2;
			postScriptFragment << 0 << " " << to_string((*shape)->get_height()/2) << " translate\n";
		}
	}
	if (get_numShapes() > 1) {
		postScriptFragment << "0 " << to_string(-relativeCurrentPoint) << " translate\n";
	}
	return postScriptFragment;
}

Scaled::Scaled(Shape &shape, pair<double, double> scaleFactor)
    : _originalShape(&shape), _scaleFactor(move(scaleFactor))
{}

double Scaled::get_width() const
{
	return _originalShape->get_width();
}
double Scaled::get_height() const
{
	return _originalShape->get_height();
}


std::stringstream Scaled::generate()
{
    std::stringstream output;
    output << "gsave" << std::endl;
	output << std::to_string(_scaleFactor.first) << " " << std::to_string(_scaleFactor.second) << " scale" << std::endl;
    output << _originalShape->generate().str();
    output << "grestore" << std::endl;

    return output;
}

}


