#include "player.h"
Player::Player(){
    Position = glm::vec3(0, -1.0f, 0);
    Front = glm::vec3(0, 0, 1.0f);
    Speed = 1;
}