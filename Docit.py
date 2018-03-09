import sys
import os
import clang.cindex

translation_unit = None


def read_file_lines(filename):
    content = []
    with open(filename) as file:
        for line in file:
            content.append(line)
    return content


def is_private_node(node):
    return node.spelling.find("_") == 0


def process_node(node, namespace, inside_namespace, file_content):
    if inside_namespace and node.kind.is_declaration() and not is_private_node(node):
        file_content.append(node)

    if node.kind == clang.cindex.CursorKind.NAMESPACE and node.spelling == namespace:
        for child in node.get_children():
            process_node(child, namespace, True, file_content)
    else:
        for child in node.get_children():
            process_node(child, namespace, inside_namespace, file_content)


def is_documentation_comment(comment):
    return comment.spelling.find("/**") == 0


def is_markdown_comment(comment):
    return comment.spelling.find("[[markdown]]") != -1


def extract_comments(translation_unit):
    comments = []
    for token in translation_unit.get_tokens(extent=translation_unit.cursor.extent):
        if token.kind == clang.cindex.TokenKind.COMMENT and is_documentation_comment(token):
            comments.append(token)
    return comments


def extract_node_by_extent(node, file_lines):
    if node.kind == clang.cindex.CursorKind.CLASS_TEMPLATE or node.kind == clang.cindex.CursorKind.CLASS_TEMPLATE_PARTIAL_SPECIALIZATION:
        return "".join(file_lines[node.extent.start.line: node.extent.start.line + 1])
    elif node.kind == clang.cindex.CursorKind.CLASS_DECL or node.kind == clang.cindex.CursorKind.STRUCT_DECL:
        return "".join(file_lines[node.extent.start.line: node.extent.start.line + 1])
    else:
        return "".join(file_lines[node.extent.start.line - 1: node.extent.end.line])


def clean_code_indentation(code_str):
    # removes the indentation of the function
    lines = []
    for line in code_str.splitlines():
        lines.append(line.strip())
    return "\n".join(lines)


def format_comment(node):
    comment_lines = []
    for line in node.spelling.splitlines():
        if line.find("[[markdown]]") != -1:
            continue
        text = line[line.find("*") + 1:].strip()
        text = text.replace("@brief", "- **brief:**")
        text = text.replace("@param[in]", "- **param[in]:**")
        text = text.replace("@param[out]", "- **param[out]:**")
        text = text.replace("@param", "- **param:**")
        text = text.replace("@tparam", "- **tparam:**")
        text = text.replace("@return", "- **return:**")

        if len(text) > 0:
            comment_lines.append(text)
    return "\n".join(comment_lines[1:len(comment_lines) - 1])


def get_markdown_decorations(node):
    decoration = "#"
    node_it = node.semantic_parent
    while True:
        if node_it.kind == clang.cindex.CursorKind.STRUCT_DECL or node_it.kind == clang.cindex.CursorKind.CLASS_DECL or node_it.kind == clang.cindex.CursorKind.CLASS_TEMPLATE or node_it.kind == clang.cindex.CursorKind.CLASS_TEMPLATE_PARTIAL_SPECIALIZATION:
            decoration += "#"
        node_it = node_it.semantic_parent
        if node_it is None:
            break
    if node.kind == clang.cindex.CursorKind.FUNCTION_DECL or node.kind == clang.cindex.CursorKind.CXX_METHOD or node.kind == clang.cindex.CursorKind.FUNCTION_TEMPLATE:
        if node.is_static_method():
            decoration += "Static Function"
        else:
            decoration += "Function"
    elif node.kind == clang.cindex.CursorKind.CONSTRUCTOR:
        if node.is_copy_constructor():
            decoration += "Copy "
        elif node.is_move_constructor():
            decoration += "Move "
        decoration += "Constructor"
    elif node.kind == clang.cindex.CursorKind.STRUCT_DECL or node.kind == clang.cindex.CursorKind.CLASS_DECL or node.kind == clang.cindex.CursorKind.CLASS_TEMPLATE or node.kind == clang.cindex.CursorKind.CLASS_TEMPLATE_PARTIAL_SPECIALIZATION:
        decoration += "Struct"
    elif node.kind == clang.cindex.CursorKind.UNION_DECL:
        decoration += "Union"

    decoration += " `" + node.spelling + "`"
    return decoration


def deduce_markdown_filename(path):
    filename = os.path.splitext(os.path.basename(path))[0]
    filename += ".md"
    return filename


def export_markdown(file_content, filepath, file_lines):
    output = []
    # loop on the file content
    for i in range(len(file_content)):
        comment_node = file_content[i]
        if comment_node.kind == clang.cindex.TokenKind.COMMENT:
            # This is a markdown comment so we need to embed it no matter what
            if is_markdown_comment(comment_node):
                output.append(format_comment(comment_node))
                continue

            # This is a documentation comment so we check if there's an actual code under it
            comment_text = format_comment(comment_node)
            code_text = []
            title_text = ""
            i += 1
            # check to see if there's a declaration under this comment then we print it
            while i < len(file_content):
                code_text.append(extract_node_by_extent(file_content[i], file_lines).strip())
                # we break when we find a node underneath that's not a template parameter nor a another comment
                # not a (template<typename T>) nor another c++ comment
                # thus when we see a template parameter we print it and continue to the next node thus printing the struct itself
                # template<typename T>
                # struct Dynamic_Array;
                if file_content[i].kind != clang.cindex.CursorKind.TEMPLATE_TYPE_PARAMETER and file_content[
                    i].kind != clang.cindex.TokenKind.COMMENT:
                    title_text = get_markdown_decorations(file_content[i])
                    break
                i += 1
            # Add the comment and the code in their correct order
            output.append(title_text)
            output.append("```C++")  # start the c++ code area
            output.append(clean_code_indentation("\n".join(code_text)))  # clean code indentation and write it
            output.append("```")
            output.append(comment_text)  # write the comment under the declaration
            output.append("")

    if len(output) == 0:
        return
    abs_path = os.path.abspath(filepath)
    if not os.path.exists(os.path.dirname(abs_path)):
        os.makedirs(os.path.dirname(abs_path))

    with open(abs_path, "w+") as file:
        file.write("\n".join(output))
    # print(abs_path)
    # print("\n".join(output))


def print_help():
    print("Docit.py gen <cpp_source_path> <namespace> <output_path>\n")
    print("\t<cpp_source_path> path of the C++ file to process\n")
    print("\t<namespace> namespace of interest\n")
    print("\t<output_path> the path to save the generated markdown documentation to\n")
    print("Docit.py help\n")
    print("\tPrints this message\n")
    print("Docit.py version\n")
    print("\tPrints the version of this program\n")


def print_version():
    print("Docit Version 1.0.0")


def process(source_path, namespace, output_path):
    # create and parse the file with libclang
    index = clang.cindex.Index.create()
    translation_unit = index.parse(source_path, args=['-std=c++14'])

    # extract the comments and the nodes
    file_content = extract_comments(translation_unit)
    process_node(translation_unit.cursor, namespace, False, file_content)

    # read the file lines
    file_lines = read_file_lines(source_path)

    # sort the file content by line number
    file_content = sorted(file_content, key=lambda node: node.location.line)

    export_markdown(file_content, output_path, file_lines)

def generate_markdown_output_path(filename):
    search_str = "include"
    filename = filename[filename.find(search_str) + len(search_str) + 1:]
    filename = os.path.join("docs", filename)
    filename = os.path.join(os.path.dirname(filename), os.path.splitext(os.path.basename(filename))[0]+".md")
    filename = os.path.abspath(filename)
    return filename

if __name__ == "__main__":
    if len(sys.argv) <= 1:
        print_help()
        exit(-1)

    first = sys.argv[1]

    if first == "gen":
        if len(sys.argv) != 5:
            print_help()
            exit(-1)
        process(sys.argv[2], sys.argv[3], sys.argv[4])
        exit(0)
    elif first == "autodoc":
        if len(sys.argv) != 4:
            print_help()
            exit(-1)
        process(sys.argv[2], sys.argv[3], generate_markdown_output_path(sys.argv[2]))
        exit(0)
    elif first == "help":
        print_help()
        exit(0)
    elif first == "version":
        print_version()
        exit(0)
    else:
        print_help()
        exit(-1)