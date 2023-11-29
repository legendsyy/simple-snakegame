#include <iostream>
#include <cmath>
#include <windows.h>
#include <chrono>



enum class Direction { None, Up, Down, Left, Right };

class Position {
  public:
      Position(int x = 5, int y = 10) : x(x), y(y) {}


      int getX() const { return x; }
      int getY() const { return y; }


      void setX(int newX) { x = newX; }
      void setY(int newY) { y = newY; }

  private: 
    int x; 
    int y; 
}; 


class Snake {
private:
    Position pos;
    int score = 0; 

public:
    Snake(int x, int y) : pos(x, y) {}

    int getSnakeX() const { return pos.getX(); }
    int getSnakeY() const { return pos.getY(); }
    int getScore() const {return score; }
    
    void setSnakePosition(int x, int y) {
        pos.setX(x);
        pos.setY(y);
    }

    void updateScore() {
      score += 1; 
    }
};

class Food {
  private: 
    Position pos; 
  
  public: 
    Food(int x, int y) : pos(x, y) {}
    int getFoodX() const { return pos.getX(); }
    int getFoodY() const { return pos.getY(); }

    void generateFood(int x_axis, int y_axis, const Snake& snake) {
      int newX, newY;
        do {
            newX = rand() % (x_axis - 2) + 2;
            newY = rand() % (y_axis - 2) + 2;
        } while (newX == snake.getSnakeX() && newY == snake.getSnakeY());
        pos.setX(newX);
        pos.setY(newY);
    }
}; 


void Draw(const Snake& snake, const Food& food, int x_axis, int y_axis) {
  system("cls"); 
  int snake_x = snake.getSnakeX();
  int snake_y = snake.getSnakeY();
  int food_x = food.getFoodX();
  int food_y = food.getFoodY(); 
  for (int row = 1; row <= x_axis; row++) {
    for (int col = 1; col <= y_axis; col++) {
      if (row == 1 || row == x_axis || col == 1 || col == y_axis) {
        std::cout << "#"; 
      } else if (row == snake_x && col == snake_y) {
        std::cout << "o"; 
      } else if (row == food_x && col == food_y) {
        std::cout << "x"; 
      } else {
        std::cout << " ";
      }


    }
    std::cout << std::endl; 
  }
  std::cout << "Current score " << snake.getScore() << std::endl; 
}

bool isOverBorder(const Snake& snake, int x_axis, int y_axis) {
    return snake.getSnakeX() <= 1 || snake.getSnakeX() >= x_axis  ||
           snake.getSnakeY() <= 1 || snake.getSnakeY() >= y_axis;
}


void Move(Snake& snake, Direction& lastDirection, std::chrono::steady_clock::time_point& lastMoveTime) {

    auto now = std::chrono::steady_clock::now();
    auto timeSinceLastMove = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastMoveTime);

    if (GetAsyncKeyState('W') & 0x8000) {
        lastDirection = Direction::Up;
    } else if (GetAsyncKeyState('S') & 0x8000) {
        lastDirection = Direction::Down;
    } else if (GetAsyncKeyState('A') & 0x8000) {
        lastDirection = Direction::Left;
    } else if (GetAsyncKeyState('D') & 0x8000) {
        lastDirection = Direction::Right;
    }
    if (timeSinceLastMove.count() >= 200) {
      switch (lastDirection) {
          case Direction::Up:
              snake.setSnakePosition(snake.getSnakeX() - 1, snake.getSnakeY());
              break;
          case Direction::Down:
              snake.setSnakePosition(snake.getSnakeX() + 1, snake.getSnakeY());
              break;
          case Direction::Left:
              snake.setSnakePosition(snake.getSnakeX(), snake.getSnakeY() - 1);
              break;
          case Direction::Right:
              snake.setSnakePosition(snake.getSnakeX(), snake.getSnakeY() + 1);
              break;
          case Direction::None:
            break; 
      }
      lastMoveTime = now; 
    }
}



int main() {
  Snake snake(5, 10); 
  Food food(6, 16); 
  const int x_axis = 10; 
  const int y_axis = 20; 
  auto lastMoveTime = std::chrono::steady_clock::now(); 
  Direction lastDirection = Direction::None;
  while (true)
  {
    if(isOverBorder(snake, x_axis, y_axis)) {
      break;
    }
    if (snake.getSnakeX() == food.getFoodX() && snake.getSnakeY() == food.getFoodY()) {
      food.generateFood(x_axis, y_axis, snake);
      snake.updateScore(); 
    }
    Move(snake, lastDirection, lastMoveTime); 
    Draw(snake, food,  x_axis, y_axis);

    


    Sleep(10);
  }
  std::cout << "--------------------" <<std::endl; 
  std::cout << "You lost with score " << snake.getScore() << std::endl; 
  Sleep(5000); 
  return 0; 
}