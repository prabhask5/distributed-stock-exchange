class MessageParser:

    def __init__(self, requiredFieldsComponents, component_parser):
        self.requiredFieldsComponents = requiredFieldsComponents
        self.component_parser = component_parser
        self.message_list = []
        self.message_dep_dict = {}
        self.message_dict = {}
        self.messages = dict()
        self.message_dep_dict = dict()
        self.internal_dependences = []

    def parse_messages(self, quick_fix_specs, component_dependencies):

        for item in quick_fix_specs.find("messages"):
            message_name = item.get("name")
            if message_name not in self.requiredFieldsComponents.required_messages:
                continue

            print("Processing Message : " + message_name)

            message_list_comp = []

            required_fields = self.requiredFieldsComponents.required_fields

            message_entity = self.component_parser.parse_components_rec(
                message_name,
                item,
                self.messages,
                message_list_comp,
                required_fields,
                True,
            )
            current_dependency_list = message_list_comp

            for component_dependency in message_list_comp:
                self.populate_component_dependency_list(
                    component_dependency,
                    current_dependency_list,
                    component_dependencies,
                )

            message_list_comp.append("Header")
            self.message_list.append(current_dependency_list)

            self.message_dep_dict[message_name] = message_list_comp
            self.message_dict[message_name] = message_entity

    def populate_component_dependency_list(
        self, component_dependency_name, current_depency_list, component_dependencies
    ):

        print("Checking dependency [" + component_dependency_name + "]")
        if component_dependency_name in component_dependencies.keys():

            if component_dependency_name not in current_depency_list:
                current_depency_list.append(component_dependency_name)

            component_depenecy_list = component_dependencies.get(
                component_dependency_name
            )

            for component_depenecy_list_component_name in component_depenecy_list:
                self.populate_component_dependency_list(
                    component_depenecy_list_component_name,
                    current_depency_list,
                    component_dependencies,
                )

    def populate_all_dependencies(self, name):

        if name not in self.internal_dependences:
            self.internal_dependences.append(name)

        if name in self.message_dep_dict.keys():
            msg_deps = self.message_dep_dict[name]
            for msg_dep in msg_deps:
                self.populate_all_dependences(self, msg_dep)

    def store_idls(self, message_name, message_contexts, dependency_list):
        idl_file = open("../idl/" + message_name + ".idl", "w")
        for dependency_name in dependency_list:
            idl_file.write('#include "' + dependency_name + '.idl"\n')

            if dependency_name not in self.component_parser.complete_dependency_list:
                self.component_parser.complete_dependency_list.append(dependency_name)

        idl_file.write("\n")

        idl_file.write("module DistributedStockExchange_" + message_name + "\n")

        idl_file.write("{\n\n")
        idl_file.write(message_contexts.get_entity_idl())
        idl_file.write("};\n")
        idl_file.close()

    def store_logger(self, message_name, message_contexts, dependency_list):

        logger_file = open("../idl/" + message_name + "Logger.hpp", "w")

        logger_file.write("#pragma once\n")

        for dependency_name in dependency_list:
            logger_file.write('#include "' + dependency_name + 'Logger.hpp"\n')

            if dependency_name not in self.component_parser.complete_dependency_list:
                self.component_parser.complete_dependency_list.append(dependency_name)

        logger_file.write(";\n")
        logger_file.write(
            message_contexts.get_entity_logger("DistributedStockExchange_")
        )
        logger_file.write("\n};\n")
        logger_file.close()

    def store_hpp_cpp(self, message_name, message_contexts, dependency_list):

        hpp_file = open("../idl/" + message_name + "Adapter.hpp", "w")
        cpp_file = open("../idl/" + message_name + "Adapter.cpp", "w")
        cpp_file.write('#include "' + message_name + 'Adapter.hpp"\n')
        cpp_file.write("#include <ConvertUtils.h>\n")

        self.component_parser.complete_dependency_list.append(message_name)
        hpp_file.write("#pragma once\n")

        hpp_file.write('#include "' + message_name + '.hpp"\n')
        hpp_file.write("#include <quickfix/Message.h>\n\n")

        for dependency_name in dependency_list:
            hpp_file.write('#include "' + dependency_name + 'Adapter.hpp"\n')

            if dependency_name not in self.component_parser.complete_dependency_list:
                self.component_parser.complete_dependency_list.append(dependency_name)

        hpp_file.write(message_contexts.get_entity_hpp("DistributedStockExchange_"))
        cpp_file.write(message_contexts.get_entity_cpp("DistributedStockExchange_"))

        hpp_file.close()

    def store_message(self):

        for message_name, message_contexts in self.message_dict.items():

            if message_name not in self.requiredFieldsComponents.required_messages:
                continue

            dependency_list = self.message_dep_dict[message_name]

            self.store_idls(message_name, message_contexts, dependency_list)
            self.store_hpp_cpp(message_name, message_contexts, dependency_list)
            self.store_logger(message_name, message_contexts, dependency_list)
