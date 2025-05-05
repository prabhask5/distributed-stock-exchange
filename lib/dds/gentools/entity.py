class Entity:

    def __init__(self, components_parser, entity_name, item, is_message):
        self.entity_name = entity_name
        self.item = item
        self.members = []
        self.groups = []
        self.is_message = is_message
        self.component_parser = components_parser

    def add_member(self, member):
        self.members.append(member)

    def add_group(self, entity):
        self.groups.append(entity)

    def get_entity_idl(self):

        comp_str = ""

        for group in self.groups:
            group_idl = group.get_entity_idl()
            comp_str += group_idl

        if self.item.tag == "message":
            comp_str += (
                "\t\n\tstruct " + self.entity_name + "\n\t{\n\t\tstring Source;\n "
                "    \t\tstring Destination;\n"
                "    \t\tstring SourceUser;\n"
                "    \t\tstring DestinationUser;\n\n\n\t\tDistributedStockExchange::Header fix_header;\n\n"
            )
        elif self.item.tag == "group":
            comp_str += "\tstruct " + self.entity_name + "\n\t{\n"
        else:
            comp_str += (
                "module DistributedStockExchange\n{\n\tstruct "
                + self.entity_name
                + "\n\t{\n"
            )

        for member in self.members:
            comp_str += member.get_idl_statement()

        if self.item.tag == "message":
            comp_str += "\t};\n"
        elif self.item.tag == "group":
            comp_str += (
                "\t};\n\ttypedef sequence<"
                + self.entity_name
                + "> "
                + self.entity_name
                + "Seq;\n\n"
            )
        else:
            comp_str += "\t};\n};"

        return comp_str

    # class prefix DistributedStockExchange for messages
    def get_entity_logger(self, class_prefix):

        comp_str = "\n\nclass " + self.entity_name + "Logger\n{\n\tpublic:\n"
        if self.is_message:
            comp_str += (
                "\t\tstatic void log(std::ostream & out, "
                + class_prefix
                + self.entity_name
                + "::"
                + self.entity_name
                + '& ddsMsg )  __attribute__ ((visibility ("default")))\n'
            )
            comp_str += (
                '\t\t{\n\t\t\t;out<< "Message : '
                + self.entity_name
                + ' { " << std::endl;\n'
            )

            comp_str += (
                '\n\t\t\t;out \n\t\t\t << "ddsMsg.Source : " << ddsMsg.Source() << '
                "std::endl "
            )
            comp_str += '\n\t\t\t << "ddsMsg.Destination : " << ddsMsg.Destination() << std::endl'
            comp_str += (
                '\n\t\t\t << "ddsMsg.SourceUser : " << ddsMsg.SourceUser() << std::endl'
            )
            comp_str += '\n\t\t\t << "ddsMsg.DestinationUser : " << ddsMsg.DestinationUser() << std::endl;'
            comp_str += "\n\n\t\t\tHeaderLogger::log(out, ddsMsg.fix_header());\n"
        else:
            comp_str += (
                "\t\tstatic void log(std::ostream & out, DistributedStockExchange::"
                + self.entity_name
                + '& ddsMsg )  __attribute__ ((visibility ("default")))\n\t\t{\n'
            )
            comp_str += '\n\t\t\t;out<< "' + self.entity_name + ' { "  << std::endl\n'

        comp_str += "\n\t\t\t;out\n"

        for member in self.members:
            comp_str += "\t\t\t"
            comp_str += member.get_logger_statement(self.entity_name)
            comp_str += "\n"

        # comp_str += " out <<std::endl";
        comp_str += ';\t\t\tout << "}";\n\t\tout << std::endl;};\n'

        return comp_str

    # class prefix DistributedStockExchange for messages
    def get_entity_hpp(self, class_prefix):
        comp_str = "\n\nclass " + self.entity_name + "Adapter\n{\n\tpublic:\n"
        if self.is_message == True:
            comp_str += (
                "\t\tstatic void FIX2DDS(const FIX::Message&, "
                + class_prefix
                + self.entity_name
                + "::"
                + self.entity_name
                + '& ddsMsg )  __attribute__ ((visibility ("default")));\n'
            )
            comp_str += (
                "\t\tstatic void DDS2FIX(const "
                + class_prefix
                + self.entity_name
                + "::"
                + self.entity_name
                + '& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));\n'
            )
        else:
            comp_str += (
                "\t\tstatic void FIX2DDS(const FIX::FieldMap&, DistributedStockExchange::"
                + self.entity_name
                + '& ddsMsg )  __attribute__ ((visibility ("default")));\n'
            )
            comp_str += (
                "\t\tstatic void DDS2FIX(const DistributedStockExchange::"
                + self.entity_name
                + '& ddsMsg, FIX::FieldMap&)  __attribute__ ((visibility ("default")));\n'
            )

        comp_str += "\n};"
        return comp_str

    def get_entity_cpp(self, class_prefix):

        if self.is_message == True:
            comp_str = (
                "\n\nvoid "
                + self.entity_name
                + "Adapter::FIX2DDS(const FIX::Message& fixMsg, "
                + class_prefix
                + self.entity_name
                + "::"
                + self.entity_name
                + "& ddsMsg )\n{\n"
            )
            comp_str += (
                "\tHeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.fix_header());\n\n"
            )
        else:
            comp_str = "\n\n"  # + self.group_templates + "\n";
            comp_str += (
                "void "
                + self.entity_name
                + "Adapter::FIX2DDS(const FIX::FieldMap& fixMsg, DistributedStockExchange::"
                + self.entity_name
                + "& ddsMsg )\n{\n"
            )

        fix2dds = ""
        dds2fix = ""
        for member in self.members:
            fix2dds_decl, dds2fix_decl = member.get_cpp_declare_statement()
            fix2dds += fix2dds_decl
            dds2fix += dds2fix_decl

        comp_str += fix2dds
        comp_str += "\n};\n\n"

        # comp_str += "\n\nvoid " + self.entity_name+"Adapter::DDS2FIX(const "+class_prefix + self.entity_name + "::"+ self.entity_name+"& ddsMsg, FIX::FieldMap& fixMsg)\n{\n";
        if self.is_message == True:
            comp_str += (
                "\n\nvoid "
                + self.entity_name
                + "Adapter::DDS2FIX(const "
                + class_prefix
                + self.entity_name
                + "::"
                + self.entity_name
                + "& ddsMsg, FIX::Message& fixMsg)\n{\n"
            )
            comp_str += (
                "\tHeaderAdapter::DDS2FIX(ddsMsg.fix_header(), fixMsg.getHeader());\n\n"
            )
        else:
            comp_str += (
                "\n\nvoid "
                + self.entity_name
                + "Adapter::DDS2FIX( const DistributedStockExchange::"
                + self.entity_name
                + "& ddsMsg, FIX::FieldMap& fixMsg)\n{\n"
            )
        comp_str += dds2fix
        comp_str += "\n};\n\n"

        return comp_str
