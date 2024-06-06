#include "object.hpp"

class Solver {
    private:
        std::vector<VerletObject> objects;
        float link_distance;
        float substeps;

        sf::VertexArray applyLink(VerletObject& obj1, VerletObject& obj2) {
            float target_distance = obj1.obj.getRadius() + obj2.obj.getRadius() + link_distance;

            Vec2 axis = obj1.position - obj2.position;
            float distance = pow(pow(axis.x, 2) + pow(axis.y, 2), 0.5);
            Vec2 n = axis / distance;
            float delta = target_distance - distance;

            if (obj1.immovable || obj2.immovable) {
                obj1.immovable ? obj2.position -= delta * n : obj1.position += delta * n;
            } else {
                obj1.position += 0.5f * delta * n;
                obj2.position -= 0.5f * delta * n;
            }
            
            sf::VertexArray line(sf::LinesStrip, 2);
            line[0].position = obj1.position;
            line[1].position = obj2.position;

            return line;
        }

    public:
        Solver() {
            VerletObject origin;
            link_distance = 20;
            substeps = 8;

            objects.push_back(VerletObject(Vec2(origin.position.x, origin.position.y), true));
            for (int i = 1; i <= 7; i++) {
                objects.push_back(VerletObject(Vec2(origin.position.x + (origin.obj.getRadius()*2 + link_distance)*i, 500)));
            }
        }

        void renderObjects(sf::RenderWindow* target, float dt) {
            float sub_dt = dt / substeps;

            for (int s = 0; s < substeps; s++) {
                for (int i = 0; i < objects.size(); i++) {
                    if (i < objects.size() - 1) target->draw(applyLink(objects[i], objects[i+1]));
                    target->draw(objects[i].obj);
                    if (!objects[i].immovable) {
                        objects[i].update(sub_dt);
                        objects[i].accelerate(Vec2(0, 1000));
                    }
                }
            }
        }
};