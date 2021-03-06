// compoundshape.hpp
//
// Created by Mark, Bryant and Jacob on 3/20/2019.
//

#ifndef CS372_CPS_COMPOUNDSHAPE_H
#define CS372_CPS_COMPOUNDSHAPE_H

#include <vector>
#include <utility> // pair
#include <functional>

#include "shape.hpp"

namespace cps
{

    class CompoundShape : public Shape
    {
    public:
        using iterator = std::vector<Shape_ptr>::iterator;
        using const_iterator = std::vector<Shape_ptr>::const_iterator;

        explicit CompoundShape(std::vector<Shape_ptr> shapes);

        void set_width(double) override
        {}

        void set_height(double) override
        {}

        std::stringstream generate() override;

        virtual std::string moveToNextShape(Shape &, double &) = 0;

        virtual std::string moveBackToOrigin(double &) = 0;

        double get_width() override;

        double get_height() override;

        virtual std::function<double (double, Shape_ptr&)> lambdaWidth() = 0;

        virtual std::function<double (double, Shape_ptr&)> lambdaHeight() = 0;

        void pushShape(Shape_ptr shape);

        size_t get_numShapes() const;

        iterator begin();

        iterator end();

        const_iterator begin() const;

        const_iterator end() const;

    private:
        std::vector<Shape_ptr> _shapes{};
    };

    class LayeredShapes : public CompoundShape
    {
    public:
        explicit LayeredShapes(std::vector<Shape_ptr> shapes = {});

        std::function<double (double, Shape_ptr&)> lambdaWidth() override;

        std::function<double (double, Shape_ptr&)> lambdaHeight() override;

        std::string moveToNextShape(Shape &, double &) override;

        std::string moveBackToOrigin(double &) override;

    private:
    };

    class HorizontalShapes : public CompoundShape
    {
    public:
        explicit HorizontalShapes(std::vector<Shape_ptr> shapes = {});

        std::function<double (double, Shape_ptr&)> lambdaWidth() override;

        std::function<double (double, Shape_ptr&)> lambdaHeight() override;

        std::string moveToNextShape(Shape &, double &) override;

        std::string moveBackToOrigin(double &) override;

    private:

    };

    class VerticalShapes : public CompoundShape
    {
    public:
        explicit VerticalShapes(std::vector<Shape_ptr> shapes = {});

        std::function<double (double, Shape_ptr&)> lambdaWidth() override;

        std::function<double (double, Shape_ptr&)> lambdaHeight() override;

        std::string moveToNextShape(Shape &, double &) override;

        std::string moveBackToOrigin(double &) override;

    private:

    };

    class Scaled : public Shape
    {
    public:
        Scaled(Shape &shape, std::pair<double, double> scaleFactor);

        double get_width() override;

        double get_height() override;

        void set_width(double) override
        {}

        void set_height(double) override
        {}

        std::stringstream generate() override;

    private:
        Shape *_originalShape;
        std::pair<double, double> _scaleFactor;
    };

}

#endif //CS372_CPS_COMPOUNDSHAPE_H
