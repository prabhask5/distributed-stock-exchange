from dds_reqs_parser import RequiredFieldsComponents
from component_parser import ComponentParser
from message_parser import MessageParser
import xml.etree.ElementTree as xml_parser


def main():
    dds_reqs = "../specs/dse_requirements.xml"
    fix_specs = "../../../fix-gateway/spec/Fix44.xml"

    print("DDS Reqs file is:", dds_reqs)
    print("FIX Specs file is:", fix_specs)

    generate(dds_reqs, fix_specs)


def generate(requirements, fix_spec):
    requiredFieldsComponents = RequiredFieldsComponents(requirements)
    fix_specs = xml_parser.parse(fix_spec)

    component_parser = ComponentParser(requiredFieldsComponents)
    message_parser = MessageParser(requiredFieldsComponents, component_parser)

    header_list_comp = []
    header_messages = dict()
    required_header_fields = []

    print("----- Processing Required Fields for QuickFIX Components -----")
    component_parser.parse_components(fix_specs)
    print("--------------------------------------------------------------\n")

    print("----- Processing Required Fields for QuickFIX Header -----")
    header_out = component_parser.parse_components_rec(
        "Header",
        fix_specs.find("header"),
        header_messages,
        header_list_comp,
        required_header_fields,
        False,
    )
    component_dependency_dictionary = component_parser.component_dep_dict
    print("--------------------------------------------------------------\n")

    print("----- Processing Required Fields for QuickFIX Message -----")
    message_parser.parse_messages(fix_specs, component_dependency_dictionary)
    print("--------------------------------------------------------------\n")

    component_parser.compenent_dict["Header"] = header_out
    component_parser.complete_dependency_list.append("Header")
    component_parser.component_dep_dict["Header"] = []

    message_parser.store_message()
    component_parser.store_components()


if __name__ == "__main__":
    main()
