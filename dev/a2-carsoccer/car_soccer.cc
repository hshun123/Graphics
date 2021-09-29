/** CSci-4611 Assignment 2:  Car Soccer
    Eric Hwang
 */

#include "car_soccer.h"
#include "config.h"


CarSoccer::CarSoccer() : GraphicsApp(1024,768, "Car Soccer") {
    // Define a search path for finding data files (images and shaders)
    searchPath_.push_back(".");
    searchPath_.push_back("./data");
    searchPath_.push_back(DATA_DIR_INSTALL);
    searchPath_.push_back(DATA_DIR_BUILD);
}

CarSoccer::~CarSoccer() {
}


Vector2 CarSoccer::joystick_direction() {
    Vector2 dir;
    if (IsKeyDown(GLFW_KEY_LEFT))
        dir[0]--;
    if (IsKeyDown(GLFW_KEY_RIGHT))
        dir[0]++;
    if (IsKeyDown(GLFW_KEY_UP))
        dir[1]++;
    if (IsKeyDown(GLFW_KEY_DOWN))
        dir[1]--;
    return dir;
}


void CarSoccer::OnSpecialKeyDown(int key, int scancode, int modifiers) {
    if (key == GLFW_KEY_SPACE) {
        // Here's where you could call some form of launch_ball();
        ball_.Reset();
    }
}


void CarSoccer::UpdateSimulation(double timeStep) {
    // Here's where you shound do your "simulation", updating the positions of the
    // car and ball as needed and checking for collisions.  Filling this routine
    // in is the main part of the assignment.
    // ball position components

    // handle ball collision with walls and the ceiling
    Point3 ball_cur_pos = ball_.position();
    float tmp_pos, tmp_vel;
    // bouncing off the ground
    if(ball_cur_pos[1] < ball_.radius()) {
      // gravity slows down the bouncing speed
      ball_.set_velocity(Vector3(ball_.velocity()[0] * 0.8,
      fabs(ball_.velocity()[1]) * 0.8, ball_.velocity()[2] * 0.8));

      ball_.set_position(Point3(ball_cur_pos[0],
        ball_.radius(), ball_cur_pos[2]));
    }


    // bouncing off the ceiling
    if(ball_cur_pos[1] > 35 - ball_.radius()) {
      // decrease the speed of ball when it hit the ceiling
      ball_.set_velocity(Vector3(ball_.velocity()[0] * 0.8,
      -fabs(ball_.velocity()[1]) * 0.8, ball_.velocity()[2] * 0.8));

      //  prevent penetrating the ceiling
      tmp_pos = 35 - ball_.radius();
      ball_.set_position(Point3(ball_cur_pos[0], tmp_pos, ball_cur_pos[2]));
    }


    // ball bouncing off from the far/close walls
    if(fabs(ball_cur_pos[2]) > 50 - ball_.radius()) {
      //
      if(ball_cur_pos[2] > 0) {
        tmp_pos = 50 - ball_.radius();
        tmp_vel = -fabs(ball_.velocity()[2] * 0.8);
      }else{
        tmp_pos = -50 + ball_.radius();
        tmp_vel = fabs(ball_.velocity()[2] * 0.8);
      }

      ball_.set_velocity(Vector3(ball_cur_pos[0] * 0.8, ball_cur_pos[1] * 0.8, tmp_vel));
      ball_.set_position(Point3(ball_cur_pos[0], ball_cur_pos[1], tmp_pos));
    }

    // ball baluncing off from the left/right walls
    if(fabs(ball_cur_pos[0]) > 40 - ball_.radius()) {
      //
      if(ball_cur_pos[0] > 0) {
        tmp_pos = 40 - ball_.radius();
        tmp_vel = -fabs(ball_.velocity()[0] * 0.8);
      }else {
        tmp_pos = -40 + ball_.radius();
        tmp_vel = fabs(ball_.velocity()[0] * 0.8);
      }
      ball_.set_velocity(Vector3(tmp_vel, ball_.velocity()[1] * 0.8, ball_.velocity()[2] * 0.8));
      ball_.set_position(Point3(tmp_pos, ball_cur_pos[1], ball_cur_pos[2]));
    }


    // comppute the distance between ball and car, find the normal vector
    Vector3 distance = ball_.position() - car_.position();
    Vector3 Norm = distance.ToUnit(); // get collisiton normal vector

    // ball & car collision interaction handling
    // computed based on the formula on handout
    if (distance.Length() < (ball_.radius() + car_.collision_radius())) {
      // update ball position
      ball_.set_position(car_.position() + (ball_.radius() + car_.collision_radius()) * Norm);

      // compute the relative velocity
      Vector3 diff = ball_.velocity() - car_.velocity();

      // update ball velocity
      ball_.set_velocity(car_.velocity() + (diff - 1.8 * (diff.Dot(Norm) * Norm)));
    }


    Vector2 dir = joystick_direction();

    // thrust drag forces
    // use sine and cosine value of XZ coordinate angles to make the circular
    // move. Ref from general thrust force formula
    // 120 and 5 indicate the propotion
    Vector3 speed = (120 * Vector3(-dir[1] * sin(car_.theta()), 0, -dir[1] * cos(car_.theta()))
                    - 4 * car_.velocity()) * timeStep;

    // update theta to handle the direction of car
    car_.set_theta(2 * (-dir[0]) * timeStep + car_.theta());

    // update car velocity
    car_.set_velocity(car_.velocity() + speed);

    // update car position
    car_.set_position(car_.position() + car_.velocity() * timeStep);

    // update ball velocity
    ball_.set_velocity(ball_.velocity() + Vector3(0, -10 * timeStep, 0));

    // Update ball position based on velocity
    ball_.set_position(ball_.position() + (ball_.velocity() * timeStep));

    // Car movement handling
    Point3 cur_car_pos = car_.position();
    float temp;

    // prevent car from penetrating the far/close walls
    if (fabs(cur_car_pos[2]) > (50 - car_.collision_radius())) {
      //std::cout << cur_car_pos[2] << std::endl;

      if (cur_car_pos[2] > 0){
        temp = 50 - car_.collision_radius();
      }else{
        temp = -50 + car_.collision_radius();
      }
      car_.set_position(Point3(cur_car_pos[0], cur_car_pos[1], temp));

      car_.set_velocity(Vector3(0, 0, 0));
    }

    // prevent car from penetrating the right/left walls
    if (fabs(cur_car_pos[0]) > 40 - car_.collision_radius() ) {

      if(cur_car_pos[0] > 0){
        temp = 40 - car_.collision_radius();
      }else{
        temp = -40 + car_.collision_radius();
      }

      car_.set_position(Point3(temp, cur_car_pos[1], cur_car_pos[2]));

      car_.set_velocity(Vector3(0, 0, 0));
    }

    // Goal !!
    if(fabs(ball_.position()[2]) >= 50.0 - ball_.radius() &&
    fabs(ball_.position()[0]) <= 10 + 1 && ball_.position()[1] < 10 + 1) {
          // reset ball position
          ball_.Reset();

          // reset car position
          car_.Reset();
    }

}


void CarSoccer::InitOpenGL() {
    // Set up the camera in a good position to see the entire field
    projMatrix_ = Matrix4::Perspective(60, aspect_ratio(), 1, 1000);
    modelMatrix_ = Matrix4::LookAt(Point3(0,60,70), Point3(0,0,10), Vector3(0,1,0));

    // Set a background color for the screen
    glClearColor(0.8,0.8,0.8, 1);

    // Load some image files we'll use
    fieldTex_.InitFromFile(Platform::FindFile("pitch.png", searchPath_));
    crowdTex_.InitFromFile(Platform::FindFile("crowd.png", searchPath_));
}


void CarSoccer::DrawUsingOpenGL() {
    // Draw the crowd as a fullscreen background image
    quickShapes_.DrawFullscreenTexture(Color(1,1,1), crowdTex_);

    // Draw the field with the field texture on it.
    Color col(16.0/255.0, 46.0/255.0, 9.0/255.0);
    Matrix4 M = Matrix4::Translation(Vector3(0,-0.201,0)) * Matrix4::Scale(Vector3(50, 1, 60));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, col);
    M = Matrix4::Translation(Vector3(0,-0.2,0)) * Matrix4::Scale(Vector3(40, 1, 50));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, Color(1,1,1), fieldTex_);

    // Draw the car
    Color carcol(0.8, 0.2, 0.2);
    Matrix4 Mcar =
        Matrix4::Translation(car_.position() - Point3(0,0,0)) *
        Matrix4::RotationY(car_.theta())* // rotate the car by Y
        Matrix4::Scale(car_.size()) *
        Matrix4::Scale(Vector3(0.5,0.5,0.5));
    quickShapes_.DrawCube(modelMatrix_ * Mcar, viewMatrix_, projMatrix_, carcol);


    // Draw the ball
    Color ballcol(1,1,1);
    Matrix4 Mball =
        Matrix4::Translation(ball_.position() - Point3(0,0,0)) *
        Matrix4::Scale(Vector3(ball_.radius(), ball_.radius(), ball_.radius()));
    quickShapes_.DrawSphere(modelMatrix_ * Mball, viewMatrix_, projMatrix_, ballcol);


    // Draw the ball's shadow -- this is a bit of a hack, scaling Y by zero
    // flattens the sphere into a pancake, which we then draw just a bit
    // above the ground plane.
    Color shadowcol(0.2,0.4,0.15);
    Matrix4 Mshadow =
        Matrix4::Translation(Vector3(ball_.position()[0], -0.1, ball_.position()[2])) *
        Matrix4::Scale(Vector3(ball_.radius(), 0, ball_.radius())) *
        Matrix4::RotationX(90);
    quickShapes_.DrawSphere(modelMatrix_ * Mshadow, viewMatrix_, projMatrix_, shadowcol);


    // You should add drawing the goals and the boundary of the playing area
    // using quickShapes_.DrawLines()


    // Draw far side goal
    std::vector<Point3> goal_Upper;

    // draw vertical net lines
    for (int i = -10; i <= 10; i++) {
      goal_Upper.push_back(Point3(float(i), 0.0, -50.0));
      goal_Upper.push_back(Point3(float(i), 10.0, -50.0));
    }
    // draw row net lines
    for (int j = 1; j <= 10; j++) {
      goal_Upper.push_back(Point3(-10.0, float(j), -50.0));
      goal_Upper.push_back(Point3(10.0, float(j), -50.0));
    }
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_,
        Color(0.9,0.4,0.9), goal_Upper, QuickShapes::LinesType::LINES, 0.1);


    // Draw close side goal
    std::vector<Point3> goal_Down;
    // draw vertical net lines
    for (int i = -10; i <= 10; i++) {
      goal_Down.push_back(Point3(float(i), 0.0, 50.0));
      goal_Down.push_back(Point3(float(i), 10.0, 50.0));
    }
    // draw row net lines
    for (int j = 1; j <= 10; j++) {
      goal_Down.push_back(Point3(-10.0, float(j), 50.0));
      goal_Down.push_back(Point3(10.0, float(j), 50.0));
    }
    quickShapes_.DrawLines(modelMatrix_ , viewMatrix_, projMatrix_,
        Color(0, 1, 1), goal_Down, QuickShapes::LinesType::LINES, 0.1);

    // right side of the box boundary
    std::vector<Point3> boundary_Right;
    boundary_Right.push_back(Point3( 40.0,  0.0, -50.0));
    boundary_Right.push_back(Point3( 40.0,  0.0,  50.0));
    boundary_Right.push_back(Point3( 40.0, 35.0,  50.0));
    boundary_Right.push_back(Point3( 40.0, 35.0, -50.0));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_,
      Color(1,1,1), boundary_Right, QuickShapes::LinesType::LINES, 0.1);

    // left side of the box boundary
    std::vector<Point3> boundary_Left;
    boundary_Left.push_back(Point3( -40.0,  0.0, -50.0));
    boundary_Left.push_back(Point3( -40.0,  0.0,  50.0));
    boundary_Left.push_back(Point3( -40.0, 35.0,  50.0));
    boundary_Left.push_back(Point3( -40.0, 35.0, -50.0));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_,
      Color(1,1,1), boundary_Left, QuickShapes::LinesType::LINES, 0.1);

    // far side of the box boundary
    std::vector<Point3> boundary_Upper;
    boundary_Upper.push_back(Point3(  40.0,  0.0, -50.0));
    boundary_Upper.push_back(Point3(  40.0, 35.0, -50.0));
    boundary_Upper.push_back(Point3( -40.0, 35.0, -50.0));
    boundary_Upper.push_back(Point3( -40.0,  0.0, -50.0));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_,
      Color(1,1,1), boundary_Upper, QuickShapes::LinesType::LINE_LOOP, 0.1);

    // close side of the box boundary
    std::vector<Point3> boundary_Down;
    boundary_Down.push_back(Point3(  40.0,  0.0,  50.0));
    boundary_Down.push_back(Point3(  40.0, 35.0,  50.0));
    boundary_Down.push_back(Point3( -40.0, 35.0,  50.0));
    boundary_Down.push_back(Point3( -40.0,  0.0,  50.0));

    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_,
      Color(1,1,1), boundary_Down, QuickShapes::LinesType::LINE_LOOP, 0.1);


}
