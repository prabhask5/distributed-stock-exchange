from entity import Entity
from member import Member, GroupMember


class ComponentParser:

    def __init__(self, requiredFieldsComponents):

        self.requiredFieldsComponents = requiredFieldsComponents
        self.complete_dependency_list = []
        self.components = dict()

        self.component_list = []
        self.component_dep_dict = {}
        self.compenent_dict = {}
        self.field_types = {}

    def populate_field_type_dict(self, quick_fix_specs):

        for item in quick_fix_specs.find("fields"):
            fix_tag_name = item.get("name")
            fix_tag_type = item.get("type")
            self.field_types[fix_tag_name] = fix_tag_type

    def parse_components(self, quick_fix_specs):

        self.populate_field_type_dict(quick_fix_specs)

        for item in quick_fix_specs.find("components"):

            component_name = item.get("name")

            component_list_comp = []
            required_fields = []

            if component_name in self.requiredFieldsComponents.required_components:
                required_fields = self.requiredFieldsComponents.required_fields

            entity = self.parse_components_rec(
                component_name,
                item,
                self.components,
                component_list_comp,
                required_fields,
                False,
            )
            self.component_list.append(component_list_comp)

            # don't add structures that have no required fields
            if len(entity.members):
                self.component_dep_dict[component_name] = component_list_comp
                self.compenent_dict[component_name] = entity

    #
    #  Checks if required by QuickFIX or if specified in requirement
    #
    def is_required(self, item):

        item_name = item.get("name")

        is_required = item.get("required")

        if is_required is not None and is_required == "Y":
            return True

        if (
            item_name not in self.requiredFieldsComponents.required_fields.keys()
            and item_name not in self.requiredFieldsComponents.required_messages.keys()
            and item_name
            not in self.requiredFieldsComponents.required_components.keys()
        ):
            return False

        return True

    def parse_components_rec(
        self, component_name, item, components, order_list, required_fields, is_message
    ):

        entity = Entity(self, component_name, item, is_message)

        for component_item in item:

            item_name = component_item.get("name")

            if not self.is_required(component_item):
                if component_item.tag == "field" and required_fields is not None:
                    if item_name not in required_fields:
                        continue
                else:
                    continue

            if component_item.tag == "field":
                if item_name == component_name:
                    item_name = item_name + "Str"
                elif item_name == "EventType":
                    item_name = item_name + "QuickFix"

            if component_item.tag == "component":

                print("\t Required FIX Component : " + component_name + "." + item_name)

                required_fields = self.requiredFieldsComponents.required_fields

                component_members = self.compenent_dict.get(item_name)

                if component_members is not None:
                    component_entity = self.parse_components_rec(
                        item_name,
                        component_members.item,
                        self.components,
                        order_list,
                        required_fields,
                        False,
                    )

                    for member in component_entity.members:
                        entity.add_member(member)

                    for group in component_entity.groups:
                        entity.add_group(group)

            elif component_item.tag == "group":

                print("\t Required FIX Group : " + component_name + "." + item_name)

                required_fields = self.requiredFieldsComponents.required_fields

                group_entity = self.parse_components_rec(
                    item_name,
                    component_item,
                    self.components,
                    order_list,
                    required_fields,
                    False,
                )
                entity.add_group(group_entity)

                member = GroupMember(item_name, item_name, component_name)
                member.add_group_components(component_item, self, self.field_types)
                entity.add_member(member)

            elif component_item.tag == "field":

                tabs = "\t"
                if is_message == True:
                    tabs = "\t\t"

                print("\t Required FIX Field : " + component_name + "." + item_name)

                type_name = self.field_types[item_name]

                if type_name in self.requiredFieldsComponents.types.keys():
                    dds_type = self.requiredFieldsComponents.types[type_name]
                    gen_requirements = None

                    if (
                        item_name
                        in self.requiredFieldsComponents.required_fields.keys()
                    ):
                        gen_requirements = (
                            self.requiredFieldsComponents.required_fields[item_name]
                        )

                    member = Member(
                        dds_type, item_name, "fixMsg", "ddsMsg", gen_requirements
                    )
                    entity.add_member(member)
                else:
                    print("Type not founds : [" + type_name + "]")

        return entity

    def store_components(self):

        for component_name in self.compenent_dict.keys():

            if component_name not in self.complete_dependency_list:
                continue

            idl_file = open("../idl/" + component_name + ".idl", "w")
            hpp_file = open("../idl/" + component_name + "Adapter.hpp", "w")
            cpp_file = open("../idl/" + component_name + "Adapter.cpp", "w")
            logger_file = open("../idl/" + component_name + "Logger.hpp", "w")

            hpp_file.write("#pragma once\n")

            hpp_file.write('#include "' + component_name + '.hpp"\n')
            hpp_file.write("#include <quickfix/Message.h>\n\n")

            cpp_file.write('#include "' + component_name + 'Adapter.hpp"\n\n')
            cpp_file.write("#include <" + "ConvertUtils.h>\n\n")

            logger_file.write("#pragma once\n")

            dependency_list = self.component_dep_dict[component_name]

            for dependency_name in dependency_list:
                idl_file.write('#include "' + dependency_name + '.idl"\n')

            component_contexts = self.compenent_dict[component_name]

            idl_file.write("\n")
            idl_file.write(component_contexts.get_entity_idl())
            idl_file.close()

            hpp_file.write(component_contexts.get_entity_hpp(""))
            hpp_file.close()

            logger_file.write(component_contexts.get_entity_logger(""))
            logger_file.write("\n};\n")
            logger_file.close()

            cpp_file.write(component_contexts.get_entity_cpp(""))
            cpp_file.close()
