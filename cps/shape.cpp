// shape.cpp
//
// Created by Mark, Bryant and Jacob on 3/20/2019.
//
#include <sstream>
using std::stringstream;
#include <cmath>
using std::cos, std::sin;

#include "shape.hpp"

#include <algorithm>
#include <random>
#include <functional>

namespace cps
{

    // Base Class
    double Shape::get_height()
    {
        return _height;
    }

    double Shape::get_width()
    {
        return _width;
    }

    void Shape::set_height(double height)
    {
        _height = height;
    }

    void Shape::set_width(double width)
    {
        _width = width;
    }

    // Circle Class
    Circle::Circle(double radius)
            : _radius{radius}
    {}

    double Circle::get_height()
    {
        return _radius * 2;
    }

    double Circle::get_width()
    {
        return _radius * 2;
    }

    void Circle::set_height(double height)
    {
        _radius = height / 2;
    }

    void Circle::set_width(double width)
    {
        _radius = width / 2;
    }

    std::stringstream Circle::generate()
    {
        return std::stringstream("0 0 " + std::to_string(_radius) + " 0 360 arc stroke\n");
    }

    // Rectangle Class
    std::stringstream Rectangle::generate()
    {
        return std::stringstream("newpath\n"
                                 + std::to_string(-1 * get_width() / 2) + " " + std::to_string(-1 * get_height() / 2) +
                                 " moveto\n"
                                 + std::to_string(get_width()) + " 0 rlineto\n"
                                 + "0 " + std::to_string(get_height()) + " rlineto\n"
                                 + std::to_string(-1 * get_width()) + " 0 rlineto\n"
                                                                      "closepath\n"
                                                                      "0 0 moveto\n"
                                                                      "stroke\n");
    }

    Rectangle::Rectangle(double width, double height)
    {
        set_height(height);
        set_width(width);
    }

    // Polygon Class
    Polygon::Polygon(int numSides, double sideLength)
    {
        _numSides = numSides;
        _sideLength = sideLength;

        if (numSides % 2 == 1)
        {
            set_height(sideLength * (1.0 + cos(pi / numSides)) / (2.0 * sin(pi / numSides)));
            set_width((sideLength * sin(pi * (numSides - 1) / (2 * numSides))) / sin(pi / numSides));
        }
        else if (numSides % 4 == 0)
        {
            set_height(sideLength * (cos(pi / numSides)) / (sin(pi / numSides)));
            set_width(get_height());
        }
        else
        {
            set_height(sideLength * (cos(pi / numSides)) / (sin(pi / numSides)));
            set_width(sideLength / sin(pi / numSides));
        }
    }

    std::stringstream Polygon::generate()
    {
        std::stringstream output;

        output << "/length " << std::to_string(_sideLength) << " def\n";
        output << "/nSides " << std::to_string(_numSides) << " def\n";
        output << "/angle { 360 nSides div } def\n";
        output << "gsave\n";
        output << std::to_string(-get_width() / 2) << " " << std::to_string(-get_height() / 2) << " translate\n";
        output << "newpath\n";
        output << "0 0 moveto\n";
        output << "0 angle 360 {\n";
        output << "length 0 lineto\n";
        output << "length 0 translate\n";
        output << "angle rotate\n";
        output << "} for\n";
        output << "closepath\n";
        output << "stroke\n";
        output << "grestore\n";

        return output;
    }

    Skyline::Skyline(int numOfBuildings)
            : _buildings{generateBuildings(numOfBuildings)}
    {
        double maxWidth = 0.0;
        for (const auto &building : _buildings)
        {
            maxWidth += building.width;
            maxWidth += building.spacing;
        }
        maxWidth += _buildings.front().spacing;

        double maxHeight = 0.0;
        if (!_buildings.empty())
        {
            maxHeight = std::max_element(
                    _buildings.begin(),
                    _buildings.end(),
                    [](auto a, auto b) { return a.height < b.height; })->height;
        }

        set_width(maxWidth);
        set_height(maxHeight);
    }

    std::stringstream Skyline::generate()
    {
        std::stringstream output;
        output << "gsave" << std::endl;
        output << (-(get_width() / 2)) << " " << (-(get_height() / 2)) << " moveto" << std::endl;
        for (auto building : _buildings)
        {
            output << building.spacing << " 0 rlineto" << std::endl;
            output << "0 " << building.height << " rlineto" << std::endl;
            output << building.width << " 0 rlineto" << std::endl;
            output << "0 " << -building.height << " rlineto" << std::endl;
        }

        output << _buildings.front().spacing << " 0 rlineto" << std::endl;
        output << "0 0 moveto" << std::endl;
        output << "stroke" << std::endl;
        output << "grestore" << std::endl;
        return output;
    }

    std::vector<Skyline::Building> Skyline::generateBuildings(int numOfBuildings)
    {
        std::random_device rd;
        std::mt19937 generator(rd());

        std::uniform_real_distribution<> randomHeight(10, 100);
        std::uniform_real_distribution<> randomWidth(20, 50);
        std::uniform_real_distribution<> randomSpacing(5, 20);

        std::vector<Building> outputVector(numOfBuildings);
        for (auto &building : outputVector)
        {
            building.height = randomHeight(generator);
            building.width = randomWidth(generator);
            building.spacing = randomSpacing(generator);
        }

        return outputVector;
    }

    Spacer::Spacer(double width, double height)
    {
        set_width(width);
        set_height(height);
    }

    std::stringstream Spacer::generate()
    {
        return std::stringstream(
                std::to_string(get_width()) + " " +
                std::to_string(get_height()) + " translate\n"
        );
    }


    Rotated::Rotated(Shape_ptr shape, int degrees) : _rotation{degrees}
    {
        if (degrees == 90 || degrees == 270)
        {
            set_height(shape->get_width());
            set_width(shape->get_height());
        }
        else
        {
            set_width(shape->get_width());
            set_height(shape->get_height());
        }

        _originalShape = std::move(shape);
    }

    std::stringstream Rotated::generate()
    {
        return std::stringstream("gsave\n"
                                 + std::to_string(_rotation) + " rotate\n"
                                 + _originalShape->generate().str()
                                 + "grestore\n");
    }

}
