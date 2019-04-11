// test_shape.cpp
//

#include <memory>
#include <string>
#include <vector>
using std::string;
using std::vector;
using std::make_unique;
using std::move;

#include "catch.hpp"
#include "../cps/shape.hpp"
#include "../cps/compoundshape.hpp"
using namespace cps;

TEST_CASE("Circle")
{
    Circle c1;
    SECTION("Getters")
    {
        REQUIRE(c1.get_height() == 0);
        REQUIRE(c1.get_width() == 0);
    }

    Circle c2(1);
    Circle c3(10000);
    Circle c4(10.9);
    SECTION("Single-Parameter Constructor")
    {
        REQUIRE(c2.get_height() == 2);
        REQUIRE(c2.get_width() == 2);

        REQUIRE(c3.get_height() == 20000);
        REQUIRE(c3.get_width() == 20000);

        REQUIRE(c4.get_height() == 21.8);
        REQUIRE(c4.get_width() == 21.8);
    }

    SECTION("PostScript Generation")
    {
        string testCircle1 = "0 0 1.000000 0 360 arc stroke\n";
        string testCircle2 = "0 0 10000.000000 0 360 arc stroke\n";
        string testCircle3 = "0 0 10.900000 0 360 arc stroke\n";
        REQUIRE(c2.generate().str() == testCircle1);
        REQUIRE(c3.generate().str() == testCircle2);
        REQUIRE(c4.generate().str() == testCircle3);
    }
}

TEST_CASE("Polygon", "[polygon]")
{
    SECTION("Triangle width")
    {
        Polygon t(3, 100);
        REQUIRE(t.get_width() == 100);
    }
    SECTION("Draw triangle")
    {
        Polygon t(3, 100);

        REQUIRE(t.generate().str() == "/length 100.000000 def\n" \
            "/nSides 3.000000 def\n" \
            "/angle { 360 nSides div } def\n" \
            "gsave\n" \
            "-50.000000 -43.301270 translate\n"
            "newpath\n" \
            "0 0 moveto\n" \
            "0 angle 360 {\n" \
            "length 0 lineto\n" \
            "length 0 translate\n" \
            "angle rotate\n" \
            "} for\n" \
            "closepath\n" \
            "stroke\n" \
            "grestore\n");
    }
}

TEST_CASE("Triangle","[triangle]")
{
    SECTION("Triangle width")
    {
        Triangle t(100);
        REQUIRE(t.get_width() == 100);
    }
    SECTION("Draw triangle")
    {
        Triangle t(100);

        REQUIRE( t.generate().str() == "/length 100.000000 def\n" \
            "/nSides 3.000000 def\n" \
            "/angle { 360 nSides div } def\n" \
            "gsave\n" \
            "-50.000000 -43.301270 translate\n" \
            "newpath\n" \
            "0 0 moveto\n" \
            "0 angle 360 {\n" \
            "length 0 lineto\n" \
            "length 0 translate\n" \
            "angle rotate\n" \
            "} for\n" \
            "closepath\n" \
            "stroke\n" \
            "grestore\n");

    }
}

TEST_CASE("Square","[square]")
{
    SECTION("Square width")
    {
        Square s(100);
        REQUIRE(s.get_width() == Approx(100).epsilon(0.01));
    }
    SECTION("Draw square")
    {
        Square s(100);

        REQUIRE( s.generate().str() == "/length 100.000000 def\n" \
            "/nSides 4.000000 def\n" \
            "/angle { 360 nSides div } def\n" \
            "gsave\n" \
            "-50.000000 -50.000000 translate\n" \
            "newpath\n" \
            "0 0 moveto\n" \
            "0 angle 360 {\n" \
            "length 0 lineto\n" \
            "length 0 translate\n" \
            "angle rotate\n" \
            "} for\n" \
            "closepath\n" \
            "stroke\n" \
            "grestore\n");

    }
}

TEST_CASE("Layered Shape")
{
    vector<CompoundShape::Shape_ptr> v2;
    v2.push_back(make_unique<Circle>(10));
    vector<CompoundShape::Shape_ptr> v3;
    v3.push_back(make_unique<Circle>(10));
    v3.push_back(make_unique<Rectangle>(10, 25));

    auto layered1 = make_unique<LayeredShapes>();
    auto layered2 = make_unique<LayeredShapes>(move(v2));
    auto layered3 = make_unique<LayeredShapes>(move(v3));

	SECTION("Width")
	{
		REQUIRE(layered1->get_width() == 0);
		REQUIRE(layered2->get_width() == 20);
		REQUIRE(layered3->get_width() == 20);
	}

	SECTION("Height")
	{
		REQUIRE(layered1->get_height() == 0);
		REQUIRE(layered2->get_height() == 20);
		REQUIRE(layered3->get_height() == 25);
    }

    SECTION("PostScript Generation")
    {
		REQUIRE(layered1->generate().str() == "");
		REQUIRE(layered2->generate().str() ==
            make_unique<Circle>(10)->generate().str() + "\n"
        );
		REQUIRE(layered3->generate().str() ==
            make_unique<Circle>(10)->generate().str() + "\n"
            + make_unique<Rectangle>(10, 25)->generate().str() + "\n"
        );
    }
}

TEST_CASE("Horizontal Shape")
{
    vector<CompoundShape::Shape_ptr> v2;
    v2.push_back(make_unique<Circle>(10));
    vector<CompoundShape::Shape_ptr> v3;
    v3.push_back(make_unique<Circle>(10));
    v3.push_back(make_unique<Circle>(10));

    auto horizontal1 = make_unique<HorizontalShapes>();
    auto horizontal2 = make_unique<HorizontalShapes>(move(v2));
    auto horizontal3 = make_unique<HorizontalShapes>(move(v3));

	SECTION("Width")
	{
		REQUIRE(horizontal1->get_width() == 0);
		REQUIRE(horizontal2->get_width() == 20);
		REQUIRE(horizontal3->get_width() == 40);
	}
	SECTION("Height")
	{
		REQUIRE(horizontal1->get_height() == 0);
		REQUIRE(horizontal2->get_height() == 20);
		REQUIRE(horizontal3->get_height() == 20);
	}
	SECTION("PostScript Generation")
	{
		REQUIRE(horizontal1->generate().str() == "");
		REQUIRE(horizontal2->generate().str() ==
            make_unique<Circle>(10)->generate().str()
            + "\n"
        );
		REQUIRE(horizontal3->generate().str() ==
            make_unique<Circle>(10)->generate().str()
            + "\n"
            + "10.000000 0 translate\n"
            + "10.000000 0 translate\n"
            + "\n"
            + make_unique<Circle>(10)->generate().str()
            + "\n"
            + "-20.000000 0 translate\n"
        );
	}
}

TEST_CASE("Vertical Shape")
{
    vector<CompoundShape::Shape_ptr> v2;
    v2.push_back(make_unique<Circle>(10));
    vector<CompoundShape::Shape_ptr> v3;
    v3.push_back(make_unique<Circle>(10));
    v3.push_back(make_unique<Circle>(10));

    auto vertical1 = make_unique<VerticalShapes>();
    auto vertical2 = make_unique<VerticalShapes>(move(v2));
    auto vertical3 = make_unique<VerticalShapes>(move(v3));

	SECTION("Width")
	{
		REQUIRE(vertical1->get_width() == 0);
		REQUIRE(vertical2->get_width() == 20);
		REQUIRE(vertical3->get_width() == 20);
	}
	SECTION("Height")
	{
		REQUIRE(vertical1->get_height() == 0);
		REQUIRE(vertical2->get_height() == 20);
		REQUIRE(vertical3->get_height() == 40);
	}
	SECTION("PostScript Generation")
	{
		REQUIRE(vertical1->generate().str() == "");
		REQUIRE(vertical2->generate().str() ==
            make_unique<Circle>(10)->generate().str()
            + "\n"
        );
		REQUIRE(vertical3->generate().str() ==
            make_unique<Circle>(10)->generate().str()
            + "\n"
            + "0 10.000000 translate\n"
            + "0 10.000000 translate\n"
            + "\n"
            + make_unique<Circle>(10)->generate().str()
            + "\n"
            + "0 -20.000000 translate\n"
        );
	}
}

TEST_CASE("Rectangle")
{
    Rectangle r1;
    Rectangle r2(1, 5);
    Rectangle r3(10, 20.1);
    Rectangle r4(10000, 400000);

    SECTION("Getters")
    {
        REQUIRE(r1.get_width() == 0);
        REQUIRE(r1.get_height() == 0);
    }

    SECTION("Two Parameter Constructor")
    {
        REQUIRE(r2.get_height() == 5);
        REQUIRE(r2.get_width() == 1);

        REQUIRE(r3.get_height() == 20.1);
        REQUIRE(r3.get_width() == 10);

        REQUIRE(r4.get_height() == 400000);
        REQUIRE(r4.get_width() == 10000);
    }

    SECTION("PostScript Generation")
    {
        REQUIRE(r2.generate().str() == "newpath\n"
                                       "-0.500000 -2.500000 moveto\n"
                                       "1.000000 0 rlineto\n"
                                       "0 5.000000 rlineto\n"
                                       "-1.000000 0 rlineto\n"
                                       "closepath\n"
                                       "0 0 moveto\n"
                                       "stroke\n");

        REQUIRE(r3.generate().str() == "newpath\n"
                                       "-5.000000 -10.050000 moveto\n"
                                       "10.000000 0 rlineto\n"
                                       "0 20.100000 rlineto\n"
                                       "-10.000000 0 rlineto\n"
                                       "closepath\n"
                                       "0 0 moveto\n"
                                       "stroke\n");

        REQUIRE(r4.generate().str() == "newpath\n"
                                       "-5000.000000 -200000.000000 moveto\n"
                                       "10000.000000 0 rlineto\n"
                                       "0 400000.000000 rlineto\n"
                                       "-10000.000000 0 rlineto\n"
                                       "closepath\n"
                                       "0 0 moveto\n"
                                       "stroke\n");


    }
}

TEST_CASE("Spacer")
{
    Spacer s1(40, 20);

    SECTION("Width and Height Correct")
    {
        REQUIRE(s1.get_width() == 40);
        REQUIRE(s1.get_height() == 20);
    }

    SECTION("PostScript Generation")
    {
        REQUIRE(s1.generate().str() == "40.000000 20.000000 translate\n");
    }
}

TEST_CASE("Rotated Shapes")
{
    std::unique_ptr<Shape> r1 = std::make_unique<Rectangle>(80, 40);
    std::unique_ptr<Shape> c1 = std::make_unique<Circle>(10);

    std::vector<std::unique_ptr<Shape>> twoShapes1;
    twoShapes1.push_back(std::make_unique<Circle>(10));
    twoShapes1.push_back(std::make_unique<Rectangle>(80, 40));

    std::vector<std::unique_ptr<Shape>> twoShapes2;
    twoShapes2.push_back(std::make_unique<Circle>(10));
    twoShapes2.push_back(std::make_unique<Rectangle>(80, 40));

    auto layer1 = std::make_unique<LayeredShapes>(std::move(twoShapes1));
    auto h1 = std::make_unique<HorizontalShapes>(std::move(twoShapes2));

    Rotated rot1(std::make_unique<Rectangle>(80, 40), 90);
    Rotated rot2(std::move(layer1), 180);
    Rotated rot3(std::move(h1), 270);

    SECTION("Width and Height changes")
    {
        REQUIRE(rot1.get_height() == r1->get_width());
        REQUIRE(rot1.get_width() == r1->get_height());

        REQUIRE(rot3.get_height() == (r1->get_width() + c1->get_width()));
        REQUIRE(rot3.get_width() == 40);
    }

    SECTION("Generate PostScript")
    {
        REQUIRE(rot1.generate().str() == "gsave\n"
                                         "90 rotate\n"
                                         "newpath\n"
                                         "-40.000000 -20.000000 moveto\n"
                                         "80.000000 0 rlineto\n"
                                         "0 40.000000 rlineto\n"
                                         "-80.000000 0 rlineto\n"
                                         "closepath\n"
                                         "0 0 moveto\n"
                                         "stroke\n"
                                         "grestore\n");

        REQUIRE(rot2.generate().str() == "gsave\n"
                                         "180 rotate\n"
                                         "0 0 10.000000 0 360 arc stroke\n"
                                         "\n"
                                         "newpath\n"
                                         "-40.000000 -20.000000 moveto\n"
                                         "80.000000 0 rlineto\n"
                                         "0 40.000000 rlineto\n"
                                         "-80.000000 0 rlineto\n"
                                         "closepath\n"
                                         "0 0 moveto\n"
                                         "stroke\n"
                                         "\n"
                                         "grestore\n");

        REQUIRE(rot3.generate().str() == "gsave\n"
                                         "270 rotate\n"
                                         "0 0 10.000000 0 360 arc stroke\n"
                                         "\n"
                                         "10.000000 0 translate\n"
                                         "40.000000 0 translate\n"
                                         "\n"
                                         "newpath\n"
                                         "-40.000000 -20.000000 moveto\n"
                                         "80.000000 0 rlineto\n"
                                         "0 40.000000 rlineto\n"
                                         "-80.000000 0 rlineto\n"
                                         "closepath\n"
                                         "0 0 moveto\n"
                                         "stroke\n"
                                         "\n"
                                         "-50.000000 0 translate\n"
                                         "grestore\n");
    }
}

TEST_CASE("Skyline")
{
    Skyline sk1(5);

    SECTION("Getters Do Not Break")
    {
        REQUIRE(sk1.get_width());
        REQUIRE(sk1.get_height());
    }
    //No further tests as results are random
}

/*
TEST_CASE("Scaled Shape")
{
    std::vector<std::unique_ptr<Shape>> shapes1;
    shapes1.push_back(std::make_unique<Circle>(20));
    Scaled sc1(std::make_unique<LayeredShapes>(std::move(shapes1)), {2, 2});

    std::vector<std::unique_ptr<Shape>> shapes2;
    shapes2.push_back(std::make_unique<Rectangle>(40, 20));
    shapes2.push_back(std::make_unique<Polygon>(6, 20));
    shapes2.push_back(std::make_unique<Circle>(40));
    Scaled sc2(std::make_unique<LayeredShapes>(std::move(shapes2)), {1.5, 3});

    std::vector<std::unique_ptr<Shape>> shapes3;
    shapes3.push_back(std::make_unique<Polygon>(8, 10));
    Scaled sc3(std::make_unique<LayeredShapes>(std::move(shapes3)), {4, 6});

    SECTION("Shape Changed Height and Width")
    {
        REQUIRE(sc1.get_width() == 80);
        REQUIRE(sc1.get_height() == 80);

        REQUIRE(sc2.get_width() == 120);
        REQUIRE(sc2.get_height() == 240);

//        REQUIRE(sc3.get_width() == 32);
    }

    SECTION("PostScript Generation")
    {
        REQUIRE(sc1.generate().str() == "gsave\n"
                                        "0 0 40.000000 0 360 arc stroke\n"
                                        "\n"
                                        "grestore\n"
        );

//        REQUIRE(sc2.generate().str() == ""
//        );
    }
}
*/