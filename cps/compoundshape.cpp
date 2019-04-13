//
// Created by Mark, Bryant and Jacob on 3/20/2019.
//

#include <numeric>
#include "compoundshape.hpp"

namespace cps
{

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

    size_t CompoundShape::get_numShapes() const
    {
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
                std::string amountToMove = moveToNextShape(**shape, relativeCurrentPoint);
                postScriptFragment << amountToMove;
                if (!amountToMove.empty())
                {
                    postScriptFragment << std::endl;
                }
            }
            postScriptFragment << (*shape)->generate().str() << std::endl;
            if (shape + 1 != end())
            {
                postScriptFragment << moveToNextShape(**shape, relativeCurrentPoint);
            }
        }
        if (get_numShapes() > 1)
        {
            postScriptFragment << moveBackToOrigin(relativeCurrentPoint);
        }
        return postScriptFragment;
    }

    double CompoundShape::get_width()
    {
        return std::accumulate(this->begin(), this->end(), 0.0, lambdaWidth());
    }

    double CompoundShape::get_height()
    {
        return 0;
    }

    LayeredShapes::LayeredShapes(std::vector<Shape_ptr> shapes)
            : CompoundShape(move(shapes))
    {}

    double LayeredShapes::get_height()
    {
        return std::accumulate(this->begin(), this->end(), 0.0,
                               [](auto a, auto &b) {
                                   if (a < b->get_height())
                                   {
                                       return b->get_height();
                                   }
                                   else
                                   {
                                       return a;
                                   }
                               });
    }

    std::string LayeredShapes::moveToNextShape(Shape &, double &)
    {
        return std::string();
    }

    std::string LayeredShapes::moveBackToOrigin(double &)
    {
        return std::string();
    }

    HorizontalShapes::HorizontalShapes(std::vector<Shape_ptr> shapes)
            : CompoundShape(move(shapes))
    {}

    std::function<double (double, CompoundShape::Shape_ptr&)> LayeredShapes::lambdaWidth()
    {
        return [](double a, Shape_ptr &b) {
            if (a < b->get_width())
            {
                return b->get_width();
            }
            else
            {
                return a;
            }
        };
    }

    double HorizontalShapes::get_height()
    {
        return std::accumulate(this->begin(), this->end(), 0.0,
                               [](auto a, auto &b) {
                                   if (a < b->get_height())
                                   {
                                       return b->get_height();
                                   }
                                   else
                                   {
                                       return a;
                                   }
                               });
    }

    std::string HorizontalShapes::moveToNextShape(Shape &shape, double &relativeCurrentPoint)
    {
        stringstream postScriptFragment;
        relativeCurrentPoint += shape.get_width() / 2;
        postScriptFragment << std::to_string(shape.get_width() / 2) << " " << "0 translate\n";
        return postScriptFragment.str();
    }

    std::string HorizontalShapes::moveBackToOrigin(double &relativeCurrentPoint)
    {
        stringstream postScriptFragment;
        postScriptFragment << to_string(-relativeCurrentPoint) << " 0 translate\n";
        return postScriptFragment.str();
    }

    std::function<double(double, Shape::Shape_ptr &)> HorizontalShapes::lambdaWidth()
    {
        return [](auto a, auto &b) { return a + b->get_width(); };
    }

    VerticalShapes::VerticalShapes(vector<Shape_ptr> shapes)
            : CompoundShape(move(shapes))
    {}

    double VerticalShapes::get_height()
    {
        return std::accumulate(this->begin(), this->end(), 0.0, [](auto a, auto &b) { return a + b->get_height(); });
    }

    std::string VerticalShapes::moveToNextShape(Shape &shape, double &relativeCurrentPoint)
    {
        stringstream postScriptFragment;
        relativeCurrentPoint += shape.get_height() / 2;
        postScriptFragment << 0 << " " << to_string(shape.get_height() / 2) << " translate\n";
        return postScriptFragment.str();
    }

    std::string VerticalShapes::moveBackToOrigin(double &relativeCurrentPoint)
    {
        stringstream postScriptFragment;
        postScriptFragment << "0 " << to_string(-relativeCurrentPoint) << " translate\n";
        return postScriptFragment.str();
    }

    std::function<double(double, Shape::Shape_ptr &)> VerticalShapes::lambdaWidth()
    {
        return [](auto a, auto &b) {
            if (a < b->get_width())
            {
                return b->get_width();
            }
            else
            {
                return a;
            }
        };
    }

    Scaled::Scaled(Shape &shape, pair<double, double> scaleFactor)
            : _originalShape(&shape), _scaleFactor(move(scaleFactor))
    {}

    double Scaled::get_width()
    {
        return _originalShape->get_width();
    }

    double Scaled::get_height()
    {
        return _originalShape->get_height();
    }


    std::stringstream Scaled::generate()
    {
        std::stringstream output;
        output << "gsave" << std::endl;
        output << std::to_string(_scaleFactor.first) << " " << std::to_string(_scaleFactor.second) << " scale"
               << std::endl;
        output << _originalShape->generate().str();
        output << "grestore" << std::endl;

        return output;
    }

}


