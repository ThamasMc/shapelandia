#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <memory>


// Small class for encapsulation of position and velocity
class Vec2
{
public:
  float x = 0;
  float y = 0;

  Vec2() {}

  Vec2(float x, float y) :
    x(x), y(y) {}
};

// Parent Shape class to be derived by Circle and Rect
class Shape
{
protected:
  std::string m_name = "Shaperson";
  Vec2 m_pos = Vec2();
  Vec2 m_speed = Vec2();
  int m_red, m_green, m_blue = 0;
  sf::Text m_label;

public:
  Shape() {}

  Shape(const std::string& name, float posX, float posY, float speedX, float speedY, int red, int green, int blue)
    : m_name(name), m_pos(posX, posY), m_speed(speedX, speedY), m_red(red), m_green(green), m_blue(blue) {}

  virtual void print()
  {
    std::cout << m_name << " " << m_pos.x << " " << m_pos.y << std::endl;
  }

  virtual sf::Shape* getSfShape() = 0;

  std::string getName()
  {
    return m_name;
  }

  Vec2 getPosition()
  {
    return m_pos;
  }

  Vec2 getSpeed()
  {
    return m_speed;
  }

  sf::Text* getLabel()
  {
    return &m_label;
  }

  void setPosition(Vec2 newPos)
  {
    m_pos = newPos;
  }

  void setSpeed(Vec2 newSpeed)
  {
    m_speed = newSpeed;
  }

};

// Unique in that it has a radius
class Circle : public Shape
{
  float m_radius = 0.0f;
  sf::CircleShape m_circle;

public:
  Circle() {}

  Circle(const std::string& name, float posX, float posY, float speedX, float speedY, int red, int green, int blue, float radius)
    : Shape(name, posX, posY, speedX, speedY, red, green, blue), m_radius(radius)
  {
    m_circle = sf::CircleShape(radius);
    m_circle.setPosition(posX, posY);
    m_circle.setFillColor(sf::Color(red, green, blue));
  }

  sf::Shape* getSfShape()
  {
    return &m_circle;
  }

};

// Unique in that is has a length and width
class Rectangle : public Shape
{
  float m_width = 0.0f;
  float m_height = 0.0f;
  sf::RectangleShape m_rect;

public:
  Rectangle() {}

  Rectangle(const std::string& name, float posX, float posY, float speedX, float speedY,int red, int green, int blue, float width, float height)
    : Shape(name, posX, posY, speedX, speedY, red, green, blue), m_width(width), m_height(height) 
  {
    m_rect = sf::RectangleShape(sf::Vector2f(width, height));
    m_rect.setPosition(posX, posY);
    m_rect.setFillColor(sf::Color(red, green, blue));
  }

  sf::Shape* getSfShape()
  {
    return &m_rect;
  }

  void print()
  {
    std::cout << "rectangle" << std::endl;
  }

};

class Font
{
public:
  std::string font_path = "fonts/default";
  int font_size = 12;
  int red, green, blue = 0;

  Font() {}

  Font(const std::string& font_path, int font_size, int red, int green, int blue)
    : font_path(font_path), font_size(font_size), red(red), green(green), blue(blue) {}

  void print()
  {
    std::cout << font_path << " " << font_size << " " << red << green << blue << std::endl;
  }
};

class Game
{
public:
  int w_width = 0;
  int w_height = 0;
  Font font;
  std::vector<std::shared_ptr<Shape>> shapes;

  void addShape(std::shared_ptr<Shape> shape)
  {
    shapes.push_back(shape);
  }

  void printShapes()
  {
    for(auto& s : shapes)
    {
      s->print();
    }
  }
  void loadFromFile(const std::string& filename)
  {
    std::ifstream fin(filename);
    std::string directive, font_path, name;
    int font_size, red, green, blue, w_width, w_height;
    float pos_x, pos_y, speed_x, speed_y, rect_width, rect_height, circ_radius;
    
    while(fin >> directive)
    {
      if (directive == "DWindow")
      {
        // Store window data
        fin >> w_width >> w_height;
        this->w_width = w_width;
        this->w_height = w_height;
      }
      else if (directive == "DFont")
      {
        // Store font data
        fin >> font_path >> red >> green >> blue >> font_size;
        this->font = Font(font_path, font_size, red, green, blue);
      }
      else if (directive == "DCircle")
      {
        // Make and add a circle to our shapes.
        fin >> name >> pos_x >> pos_y >> speed_x >> speed_y >> circ_radius >>  red >> green >> blue;
        std::shared_ptr<Shape> circle = std::make_shared<Circle>(name, pos_x, pos_y, speed_x, speed_y, red, green, blue, circ_radius);
        this->addShape(circle);
      }
      else if (directive == "DRectangle")
      {
        // Make and add a rectangle to our shapes.
        fin >> name >> pos_x >> pos_y >> speed_x >> speed_y >> rect_width >> rect_height >> red >> green >> blue;
        std::shared_ptr<Shape> rect = std::make_shared<Rectangle>(name, pos_x, pos_y, speed_x, speed_y, red, green, blue, rect_width, rect_height);
        this->addShape(rect);
      }
      else
      {
        std::cout << "Nothing found for directive: " << directive << std::endl;
      }
    }
  }
};


int main()
{
  // Let's set up the game
  Game myGame;
  myGame.loadFromFile("src/config4");

  // Handle my fonts and set up labels!
  sf::Font myFont;
  sf::Color myFontColor = sf::Color(myGame.font.red, myGame.font.green, myGame.font.blue);
  if(!myFont.loadFromFile(myGame.font.font_path))
  {
    std::cerr << "Could not load font!" << std::endl;
    exit(-1);
  }

  // Initial setup for label text on each shape.
  for (auto& shape : myGame.shapes)
  {
    sf::Text* labelText = shape->getLabel();
    labelText->setString(shape->getName());
    labelText->setFont(myFont);
    labelText->setCharacterSize(myGame.font.font_size);
    labelText->setFillColor(myFontColor);
  }

  // Let's draw everything!
  sf::RenderWindow window(sf::VideoMode(myGame.w_width, myGame.w_height), "Shapelandia");
  // window.setFramerateLimit(60);

  while (window.isOpen())
  {
    // Check all the window's events since the last iteration
    sf::Event event;
    while (window.pollEvent(event))
    {
      // Close the window on close request
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear(sf::Color::Black);
    // Iterate through our shapes and carry out game logic
    for (auto& shape : myGame.shapes)
    {
      auto currShape = shape->getSfShape();
      float currPosX = shape->getPosition().x;
      float currPosY = shape->getPosition().y;
      float currSpeedX = shape->getSpeed().x;
      float currSpeedY = shape->getSpeed().y;
      float currBoundsX = currShape->getLocalBounds().width;
      float currBoundsY = currShape->getLocalBounds().height;
      bool speedChanged = false;

      // Handle bouncing
      // When we're hitting the left
      if (currPosX <= 0)
      {
        currSpeedX *= -1;
        speedChanged = true;
      }
      // When we're hitting the right
      if ((currPosX + currBoundsX) >= myGame.w_width)
      {
        currSpeedX *= -1;
        speedChanged = true;
      }
      // When we're hitting the top
      if (currPosY <= 0)
      {
        currSpeedY *= -1;
        speedChanged = true;
      }
      // When we're hitting the bottom
      if ((currPosY + currBoundsY) >= myGame.w_height)
      {
        currSpeedY *= -1;
        speedChanged = true;
      }

      // Persist our speed changes if it changed
      if (speedChanged)
      {
        shape->setSpeed(Vec2(currSpeedX, currSpeedY));
      }

      // Calculate our updated position
      float targetX = currPosX + currSpeedX;
      float targetY = currPosY + currSpeedY;
      shape->setPosition(Vec2(targetX, targetY));
      currShape->setPosition(sf::Vector2f(targetX, targetY));

      // Calculate our centered position for text
      sf::Text* labelText = shape->getLabel();
      float labelWidth = labelText->getLocalBounds().width;
      float labelHeight = labelText->getLocalBounds().height;
      float offsetX = ((currBoundsX - labelWidth) / 2); 
      float offsetY = ((currBoundsY - labelHeight) / 2);
      labelText->setPosition(sf::Vector2f(targetX + offsetX, targetY + offsetY));

      // Draw our shapes and text!
      window.draw(*currShape);
      window.draw(*labelText);
    }

    window.display();
  }

  return 0;
}
