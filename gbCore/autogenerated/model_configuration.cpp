// autogenerated: do not add any changes
#include "model_configuration.h"
namespace gb
{
std::string model_configuration::get_mesh_filename(void) const
{
const auto& iterator = m_attributes.find("/model/mesh_filename");
assert(iterator != m_attributes.end());
std::string value; iterator->second->get(&value);
return value;
}
#if defined(__EDITOR__)
void model_configuration::set_mesh_filename(std::string mesh_filename)
{
configuration::set_attribute("/model/mesh_filename", std::make_shared<configuration_attribute>(mesh_filename));
}
#endif
i32 model_configuration::get_z_order(void) const
{
const auto& iterator = m_attributes.find("/model/z_order");
assert(iterator != m_attributes.end());
i32 value; iterator->second->get(&value);
return value;
}
#if defined(__EDITOR__)
void model_configuration::set_z_order(i32 z_order)
{
configuration::set_attribute("/model/z_order", std::make_shared<configuration_attribute>(z_order));
}
#endif
bool model_configuration::get_batching(void) const
{
const auto& iterator = m_attributes.find("/model/is_batching");
assert(iterator != m_attributes.end());
bool value; iterator->second->get(&value);
return value;
}
#if defined(__EDITOR__)
void model_configuration::set_batching(bool is_batching)
{
configuration::set_attribute("/model/is_batching", std::make_shared<configuration_attribute>(is_batching));
}
#endif
std::vector<std::shared_ptr<configuration>> model_configuration::get_materials_configurations(void) const
{
const auto& iterator = m_configurations.find("/model/materials/material");
if(iterator == m_configurations.end())
{
return std::vector<std::shared_ptr<configuration>>();
}
assert(iterator != m_configurations.end());
return iterator->second;
}
#if defined(__EDITOR__)
void model_configuration::add_materials_configurations(const std::shared_ptr<gb::material_configuration>& material)
{
configuration::set_configuration("/model/materials/material", material);
}
#endif
#if defined(__EDITOR__)
void model_configuration::set_materials_configurations(const std::shared_ptr<gb::material_configuration>& material, i32 index)
{
configuration::set_configuration("/model/materials/material", material, index);
}
#endif
std::vector<std::shared_ptr<configuration>> model_configuration::get_animations_configurations(void) const
{
const auto& iterator = m_configurations.find("/model/animations/animation");
if(iterator == m_configurations.end())
{
return std::vector<std::shared_ptr<configuration>>();
}
assert(iterator != m_configurations.end());
return iterator->second;
}
#if defined(__EDITOR__)
void model_configuration::add_animations_configurations(const std::shared_ptr<gb::animation_configuration>& animation)
{
configuration::set_configuration("/model/animations/animation", animation);
}
#endif
#if defined(__EDITOR__)
void model_configuration::set_animations_configurations(const std::shared_ptr<gb::animation_configuration>& animation, i32 index)
{
configuration::set_configuration("/model/animations/animation", animation, index);
}
#endif
void model_configuration::serialize(const std::string& filename)
{
pugi::xml_document document;
pugi::xml_parse_result result = configuration::open_xml_document(document, filename);
assert(result.status == pugi::status_ok);
pugi::xpath_node node;
node = document.select_single_node("/model");
std::string mesh_filename = node.node().attribute("mesh_filename").as_string();
configuration::set_attribute("/model/mesh_filename", std::make_shared<configuration_attribute>(mesh_filename));
i32 z_order = node.node().attribute("z_order").as_int();
configuration::set_attribute("/model/z_order", std::make_shared<configuration_attribute>(z_order));
bool is_batching = node.node().attribute("is_batching").as_bool();
configuration::set_attribute("/model/is_batching", std::make_shared<configuration_attribute>(is_batching));
pugi::xpath_node_set material_nodes = document.select_nodes("/model/materials/material");
for (pugi::xpath_node_set::const_iterator iterator = material_nodes.begin(); iterator != material_nodes.end(); ++iterator)
{
std::shared_ptr<gb::material_configuration> material = std::make_shared<gb::material_configuration>();
material->serialize((*iterator).node().attribute("filename").as_string());
configuration::set_configuration("/model/materials/material", material);
}
pugi::xpath_node_set animation_nodes = document.select_nodes("/model/animations/animation");
for (pugi::xpath_node_set::const_iterator iterator = animation_nodes.begin(); iterator != animation_nodes.end(); ++iterator)
{
std::shared_ptr<gb::animation_configuration> animation = std::make_shared<gb::animation_configuration>();
pugi::xpath_node node = (*iterator);
animation->serialize(document, node);
configuration::set_configuration("/model/animations/animation", animation);
}
}
#if defined(__EDITOR__)
void model_configuration::deserialize(const std::string& filename)
{
pugi::xml_document document;
pugi::xml_parse_result result = document.load("");
assert(result.status == pugi::status_ok);
pugi::xml_node node = document.append_child("model");
pugi::xml_node parent_node = node;
pugi::xml_attribute attribute;
attribute = node.append_attribute("mesh_filename");
std::string mesh_filename = model_configuration::get_mesh_filename();
attribute.set_value(mesh_filename.c_str());
attribute = node.append_attribute("z_order");
i32 z_order = model_configuration::get_z_order();
attribute.set_value(z_order);
attribute = node.append_attribute("is_batching");
bool is_batching = model_configuration::get_batching();
attribute.set_value(is_batching);
node = parent_node.append_child("materials");
for(const auto& iterator : model_configuration::get_materials_configurations())
{
std::shared_ptr<gb::material_configuration> configuration = std::static_pointer_cast<gb::material_configuration>(iterator);
pugi::xml_node child_node = node.append_child("material");
attribute = child_node.append_attribute("filename");
attribute.set_value(configuration->get_filename().c_str());
}
node = parent_node.append_child("animations");
for(const auto& iterator : model_configuration::get_animations_configurations())
{
std::shared_ptr<gb::animation_configuration> configuration = std::static_pointer_cast<gb::animation_configuration>(iterator);
pugi::xml_node child_node = node.append_child("animation");
configuration->deserialize(child_node);
}
document.save_file(filename.c_str());
}
#endif
}
