#pragma once

#include "LibTrace/scene.hpp"

#include "LibTrace/shapes/sphere.hpp"
#include "LibTrace/shapes/cube.hpp"

#include "LibTrace/materials/lamberitan.hpp"
#include "LibTrace/materials/metal.hpp"
#include "LibTrace/materials/light.hpp"
#include "LibTrace/materials/glass.hpp"

#include "LibTrace/textures/solid.hpp"
#include "LibTrace/textures/checker.hpp"

extern QSharedPointer<Shape> (*shape_parsers[])(const QJsonObject&);
extern QSharedPointer<Material> (*material_parsers[])(const QJsonObject&);
extern QSharedPointer<Texture> (*texture_parsers[])(const QJsonObject&);

extern verdict (*shape_checkers[])(const QJsonObject&);
extern verdict (*material_checkers[])(const QJsonObject&);
extern verdict (*texture_checkers[])(const QJsonObject&);

QSharedPointer<Shape> parse_json_shape(const QJsonObject &json, const QString &name);
verdict check_json_shape(const QJsonObject &json, const QString &name);

QSharedPointer<Material> parse_json_material(const QJsonObject &json, const QString &name);
verdict check_json_material(const QJsonObject &json, const QString &name);

QSharedPointer<Texture> parse_json_texture(const QJsonObject &json, const QString &name);
verdict check_json_texture(const QJsonObject &json, const QString &name);

QSharedPointer<FigureList> parse_json_figurelist(const QJsonObject &json, const QString &name);
verdict check_json_figurelist(const QJsonObject &json, const QString &name);

QSharedPointer<Viewport> parse_json_viewport(const QJsonObject &json, const QString &name);
verdict check_json_viewport(const QJsonObject &json, const QString &name);

QPair<QSharedPointer<Scene>, Hitinfo> parse_json_scene(const QJsonObject &json);
verdict check_json_scene(const QJsonObject &json);