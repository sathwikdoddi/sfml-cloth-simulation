#include "object.hpp"

class Solver {
    private:
        std::vector<VerletObject> objects;
        float linkDistance;
        float substeps;
        float numObjects;

        sf::VertexArray applyLink(VerletObject& obj1, VerletObject& obj2) {
            if (!obj1.isLinkedTo(&obj2)) {
                return sf::VertexArray();
            }
            float targetDistance = obj1.obj.getRadius() + obj2.obj.getRadius() + linkDistance;

            Vec2 axis = obj1.position - obj2.position;
            float distance = pow(pow(axis.x, 2) + pow(axis.y, 2), 0.5);
            Vec2 n = axis / distance;
            float delta = targetDistance - distance;

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

        void initializeObjects() {
            VerletObject origin;

            objects.push_back(VerletObject(Vec2(origin.position.x, origin.position.y), true));
            for (int i = 1; i <= numObjects; i++) {
                objects.push_back(VerletObject(Vec2(origin.position.x + (origin.obj.getRadius()*2 + linkDistance)*i, 500)));
            }

            for (int i = 0; i < objects.size() - 1; i++) {
                applyLink(objects[i], objects[i+1]);
                objects[i].links.push_back(&objects[i+1]);
            }
        }

        void removeLink(VerletObject& obj1, VerletObject& obj2) {
            for (int i = 0; i < obj1.links.size(); i++) {
                if (obj1.links[i] == &obj2)  {
                    obj1.links.erase(obj1.links.begin() + i);
                }
            }
        }

    public:
        Solver() {
            linkDistance = 20;
            substeps = 4;
            numObjects = 8;

            initializeObjects();
        }

        void renderObjects(sf::RenderWindow* target, float dt, sf::Vector2i mousePos = sf::Vector2i(0,0)) {
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
                removeAppropriateLinks(mousePos);
            }
        }

        void removeAppropriateLinks(sf::Vector2i mousePos) {
            for (int i = 0; i < objects.size(); i++) {
                VerletObject& obj1 = objects[i];

                float mouse_x = mousePos.x;
                float mouse_y = mousePos.y;
                Vec2 mouse = Vec2(mouse_x, mouse_y);

                Vec2 toMouse = obj1.position - mouse;
                float d1 = pow(pow(toMouse.x, 2) + pow(toMouse.y, 2), 0.5);
                Vec2 nToMouse = toMouse / d1;

                for (int j = i+1; j < objects.size(); j++) {
                    VerletObject& obj2 = objects[j];

                    Vec2 toObj2 = obj1.position - obj2.position;
                    float d2 = pow(pow(toObj2.x, 2) + pow(toObj2.y, 2), 0.5);
                    Vec2 nToObj2 = toObj2 / d2;

                    if (abs(nToMouse.x - nToObj2.x) <= 0.2 && abs(nToMouse.x - nToObj2.x) <= 0.2 && d1 < linkDistance) {
                        removeLink(obj1, obj2);
                    }
                }
            }
        }
};