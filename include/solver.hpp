#include "object.hpp"

class Solver {
    private:
        std::vector<std::vector<VerletObject> > objects;
        float linkDistance;
        float substeps;
        float numObjects;
        float startx;
        float starty;

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
            float speedish = 0.03;
            Vec2 velocity(speedish / numObjects, speedish / numObjects);
            VerletObject origin(Vec2(startx, starty));

            for (int i = 0; i < numObjects; i++) {
                std::vector<VerletObject> row;
                float yPos = origin.position.y + (2*origin.obj.getRadius() + linkDistance) * i;
                for (int j = 0; j < numObjects; j++) {
                    float xPos = origin.position.x + (2*origin.obj.getRadius() + linkDistance) * j;
                    VerletObject obj = VerletObject(Vec2(xPos, yPos), i == 0);
                    if (!(i == 0)) {
                        float a = i*i;
                        obj.position = obj.position + a*velocity;
                    }
                    row.push_back(obj);
                }
                objects.push_back(row);
            }

            for (int i = 0; i < numObjects; i++) {
                for (int j = 0; j < numObjects; j++) {
                    if (i < numObjects - 1) {
                        applyLink(objects[i][j], objects[i+1][j]);
                        objects[i][j].links.push_back(&objects[i+1][j]);
                    }

                    if (j < numObjects - 1) {
                        applyLink(objects[i][j], objects[i][j+1]);
                        objects[i][j].links.push_back(&objects[i][j+1]);
                    }
                }
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
        Solver(float width, float height) {
            linkDistance = 10;
            substeps = 4;
            numObjects = 30;

            VerletObject sample;
            startx = width / 2 - (numObjects / 2) * (2*sample.obj.getRadius() + linkDistance);
            starty = height / 2 - (numObjects / 2) * (2*sample.obj.getRadius() + linkDistance);

            initializeObjects();
        }

        void renderObjects(sf::RenderWindow* target, float dt, sf::Vector2i mousePos = sf::Vector2i(0,0)) {
            float sub_dt = dt / substeps;

            for (int s = 0; s < substeps; s++) {
                for (int i = 0; i < objects.size(); i++) {
                    for (int j = 0; j < objects.size(); j++) {
                        if (i < objects.size() - 1) {
                            target->draw(applyLink(objects[i][j], objects[i+1][j]));
                        }
                        if (j < objects.size()) {
                            target->draw(applyLink(objects[i][j], objects[i][j+1]));
                        }

                        target->draw(objects[i][j].obj);
                        if (!objects[i][j].immovable) {
                            objects[i][j].update(sub_dt);
                            objects[i][j].accelerate(Vec2(0, 1000));
                        }
                    }
                }

                removeAppropriateLinks(mousePos);
            }
        }

        void removeAppropriateLinks(sf::Vector2i mousePos) {
            for (int i = 0; i < objects.size(); i++) {
                for (int j = 0; j < objects.size(); j++) {
                    VerletObject& obj1 = objects[i][j];

                    float mouse_x = mousePos.x;
                    float mouse_y = mousePos.y;
                    Vec2 mouse = Vec2(mouse_x, mouse_y);

                    Vec2 toMouse = obj1.position - mouse;
                    float d1 = pow(pow(toMouse.x, 2) + pow(toMouse.y, 2), 0.5);
                    Vec2 nToMouse = toMouse / d1;

                    for (int k = 0; k < obj1.links.size(); k++) {
                        VerletObject& obj2 = *obj1.links[k];

                        Vec2 toObj2 = obj1.position - obj2.position;
                        float d2 = pow(pow(toObj2.x, 2) + pow(toObj2.y, 2), 0.5);
                        Vec2 nToObj2 = toObj2 / d2;

                        if (abs(nToMouse.x - nToObj2.x) <= 0.2 && abs(nToMouse.x - nToObj2.x) <= 0.2 && d1 < linkDistance) {
                            removeLink(obj1, obj2);
                        }
                    }
                }
            }
        }
};