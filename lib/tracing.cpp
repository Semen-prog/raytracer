#include "LibTrace/tracing.hpp"
#include <QJsonArray>

QSharedPointer<Shape> (*shape_parsers[])(const QJsonObject &) = {
    Sphere::parse_json,
    Cube::parse_json};

QSharedPointer<Material> (*material_parsers[])(const QJsonObject &){
    Lamberitan::parse_json,
    Metal::parse_json,
    Light::parse_json,
    Glass::parse_json};

QSharedPointer<Texture> (*texture_parsers[])(const QJsonObject &){
    Solid::parse_json,
    Checker::parse_json};

verdict (*shape_checkers[])(const QJsonObject &) = {
    Sphere::check_json,
    Cube::check_json};

verdict (*material_checkers[])(const QJsonObject &) = {
    Lamberitan::check_json,
    Metal::check_json,
    Light::check_json,
    Glass::check_json};

verdict (*texture_checkers[])(const QJsonObject &) = {
    Solid::check_json,
    Checker::check_json};

QSharedPointer<Shape> parse_json_shape(const QJsonObject &json, const QString &name) {
    QJsonObject obj = json[name].toObject();
    int type = parse_json_int(obj, "type");
    return shape_parsers[type](obj);
}
verdict check_json_shape(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isObject()) return verdict(false, QString("Error: no such shape field: %1\n").arg(name));
    QJsonObject obj = json[name].toObject();
    auto v = check_json_double(obj, "type", 0, 1);
    if (!v.ok) return v;
    int type = parse_json_int(obj, "type");
    return shape_checkers[type](obj);
}

QSharedPointer<Material> parse_json_material(const QJsonObject &json, const QString &name) {
    QJsonObject obj = json[name].toObject();
    int type = parse_json_int(obj, "type");
    return material_parsers[type](obj);
}
verdict check_json_material(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isObject()) return verdict(false, QString("Error: no such material field: %1\n").arg(name));
    QJsonObject obj = json[name].toObject();
    auto v = check_json_double(obj, "type", 0, 1);
    if (!v.ok) return v;
    int type = parse_json_int(obj, "type");
    return material_checkers[type](obj);
}

QSharedPointer<Texture> parse_json_texture(const QJsonObject &json, const QString &name) {
    QJsonObject obj = json[name].toObject();
    int type = parse_json_int(obj, "type");
    return texture_parsers[type](obj);
}
verdict check_json_texture(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isObject()) return verdict(false, QString("Error: no such texture field: %1\n").arg(name));
    QJsonObject obj = json[name].toObject();
    auto v = check_json_double(obj, "type", 0, 1);
    if (!v.ok) return v;
    int type = parse_json_int(obj, "type");
    return texture_checkers[type](obj);
}

QSharedPointer<FigureList> parse_json_figurelist(const QJsonObject &json, const QString &name) {
    QJsonArray arr = json[name].toArray();
    auto fl = QSharedPointer<FigureList>::create();
    for (int i = 0; i < arr.size(); ++i) {
        QJsonObject obj = arr.at(i).toObject();
        fl->add(parse_json_shape(obj, "shape"), parse_json_material(obj, "material"), parse_json_texture(obj, "texture"));
    }
    return fl;
}
verdict check_json_figurelist(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isArray()) return verdict(false, QString("Error: no such figurelist field: %1\n").arg(name));
    QJsonArray arr = json[name].toArray();
    verdict v;
    for (int i = 0; i < arr.size(); ++i) {
        if (!arr.at(i).isObject()) return verdict(false, "Error: figure is not an object\n");
        QJsonObject obj = arr.at(i).toObject();
        v = v + check_json_shape(obj, "shape") + check_json_material(obj, "material") + check_json_texture(obj, "texture");
    }
    return v;
}

QSharedPointer<Viewport> parse_json_viewport(const QJsonObject &json, const QString &name) {
    QJsonObject obj = json[name].toObject();
    return QSharedPointer<Viewport>::create(
        parse_json_int(obj, "pixel_width"),
        parse_json_int(obj, "pixel_height"),
        radian(parse_json_double(obj, "field_of_view")),
        parse_json_double(obj, "x_offset"),
        parse_json_double(obj, "y_offset"),
        parse_json_pv(obj, "lookfrom"),
        parse_json_pv(obj, "lookat"),
        parse_json_pv(obj, "dir_up"),
        radian(parse_json_double(obj, "defocus_angle")));
}
verdict check_json_viewport(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isObject()) return verdict(false, QString("Error: no such viewport field: %1\n").arg(name));
    QJsonObject obj = json[name].toObject();
    return check_json_int(obj, "pixel_width", 1) +
        check_json_int(obj, "pixel_height", 1) +
        check_json_double(obj, "field_of_view", 0, 180) +
        check_json_double(obj, "x_offset", 0) +
        check_json_double(obj, "y_offset", 0) +
        check_json_pv(obj, "lookfrom") +
        check_json_pv(obj, "lookat") +
        check_json_pv(obj, "dir_up") +
        check_json_double(obj, "defocus_angle", 0, 180);
}

QPair<QSharedPointer<Scene>, Hitinfo> parse_json_scene(const QJsonObject &json) {
    return QPair(
        QSharedPointer<Scene>::create(parse_json_figurelist(json, "figure_list"),
                                      parse_json_viewport(json, "viewport"),
                                      parse_json_int(json, "samples_per_pixel"),
                                      parse_json_int(json, "max_depth")),
        Hitinfo(Hittype(parse_json_int(json, "hittype")),
                Rendertype(parse_json_int(json, "rendertype"))));
}
verdict check_json_scene(const QJsonObject &json) {
    return check_json_figurelist(json, "figure_list") +
        check_json_viewport(json, "viewport") +
        check_json_int(json, "samples_per_pixel", 1) +
        check_json_int(json, "max_depth", 1) +
        check_json_int(json, "hittype", 0, 1) +
        check_json_int(json, "rendertype");
}