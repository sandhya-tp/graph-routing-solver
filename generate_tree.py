import os

def generate_tree(startpath, prefix=""):
    output = []
    for root, dirs, files in os.walk(startpath):
        level = root.replace(startpath, "").count(os.sep)
        indent = "│   " * level + "├── "
        output.append(f"{indent}{os.path.basename(root)}/")
        subindent = "│   " * (level + 1)
        for f in files:
            output.append(f"{subindent}└── {f}")
    return "\n".join(output)

if __name__ == "__main__":
    root_path = "/home/sandhya/Documents/Shortest_Path_Problem"  # change this to your project folder
    tree = generate_tree(root_path)
    print("```")
    print(tree)
    print("```")

