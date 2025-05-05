import xml.etree.ElementTree as xml_parser


class RequiredFieldsComponents:

    def __init__(self, filename):

        self.requirements = xml_parser.parse(filename)
        self.required_messages = {}
        self.required_components = {}
        self.required_fields = {}
        self.types = {}

        messages = self.requirements.find("messages")
        self.populate_elements(messages, self.required_messages)

        components = self.requirements.find("components")
        self.populate_elements(components, self.required_components)

        fields = self.requirements.find("fields")
        self.populate_elements(fields, self.required_fields)

        types = self.requirements.find("types")
        for type_item in types:
            fix_type = type_item.get("fix")
            self.types[fix_type] = type_item

    # elements : messages or components
    def populate_elements(self, elements, required_elements):
        for item in elements:
            element_name = item.get("name")
            required_elements[element_name] = item
