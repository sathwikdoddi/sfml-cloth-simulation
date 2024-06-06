#include "object.hpp"

class Solver {
    private:
        std::vector<VerletObject> objects;

        void applyLink(VerletObject& obj1, VerletObject& obj2) {
            float target_distance = obj1.obj.getRadius() + obj2.obj.getRadius() + 5;

            Vec2 axis = obj1.position - obj2.position;
            float distance = pow(pow(axis.x, 2) + pow(axis.y, 2), 0.5);
            Vec2 n = axis / distance;
            float delta = target_distance - distance;

            if (obj1.immovable) {
                obj2.position -= delta * n;
            } else {
                obj1.position += 0.5f * delta * n;
                obj2.position -= 0.5f * delta * n;
            }
        }

    public:
        Solver() {
            VerletObject origin;
            objects.push_back(VerletObject(Vec2(origin.position.x, origin.position.y), true));
            for (int i = 1; i <= 7; i++) {
                objects.push_back(VerletObject(Vec2(origin.position.x + 45*i, 500)));
            }
        }

        void renderObjects(sf::RenderWindow* target, float dt) {
            for (int i = 0; i < objects.size() - 1; i++) {
                applyLink(objects[i], objects[i+1]);
            }

            for (int i = 0; i < objects.size(); i++) {
                target->draw(objects[i].obj);
                if (!objects[i].immovable) {
                    objects[i].update(dt);
                    objects[i].accelerate(Vec2(0, 1000));
                }
            }
        }
};