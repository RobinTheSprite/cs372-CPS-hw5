// shape.hpp
//
// Created by Mark, Bryant and Jacob on 3/20/2019.
//

#ifndef CS372_CPS_SHAPE_H
#define CS372_CPS_SHAPE_H

#include <sstream>
#include <cmath>
#include <vector>
#include <memory>

namespace cps
{

    class Shape
    {
    public:
        using Shape_ptr = std::unique_ptr<Shape>;

        virtual ~Shape() = default;

        virtual double get_height() const;

        virtual double get_width() const;

        virtual void set_height(double height);

        virtual void set_width(double width);

        virtual std::stringstream generate() = 0;

    private:
        double _height{0};
        double _width{0};
    };


    class Circle : public Shape
    {
    public:
        Circle() = default;

        explicit Circle(double);

        double get_height() const override;

        double get_width() const override;

        void set_height(double height) override;

        void set_width(double width) override;

        std::stringstream generate() override;

    private:

        double _radius{0.0};
    };

    class Rectangle : public Shape
    {
    public:
        Rectangle() = default;

        Rectangle(double, double);

        std::stringstream generate() override;

    private:
    };

    class Spacer : public Shape
    {
    public:
        Spacer(double, double);

        std::stringstream generate() override;

    private:
    };

    class Polygon : public Shape
    {
    public:
        ~Polygon() override = default;

        Polygon(int, double);

        std::stringstream generate() override;

    private:
        const double pi = std::acos(-1);
        float _numSides{0};
        float _sideLength{0};
    };

    class Square : public Polygon
    {
    public:
        explicit Square(double sideLength) : Polygon(4, sideLength)
        {};

        ~Square() override = default;
    };

    class Triangle : public Polygon
    {
    public:
        explicit Triangle(double sideLength) : Polygon(3, sideLength)
        {};

        ~Triangle() override = default;
    };

    class Skyline : public Shape
    {
    public:
        explicit Skyline(int);

        std::stringstream generate() override;

    private:
        struct Building
        {
            double spacing;
            double height;
            double width;
        };

        static std::vector<Building> generateBuildings(int);

        std::vector<Building> _buildings;
    };

    class Rotated : public Shape
    {
    public:
        Rotated(Shape_ptr, int);

        std::stringstream generate() override;

    private:
        Shape_ptr _originalShape;
        int _rotation;
    };

}

#endif //CS372_CPS_SHAPE_H
