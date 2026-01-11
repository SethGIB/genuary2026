file_path = "raw_sphere_data.txt"
src_strs = []

with open(file_path, "r") as f:
    src_strs = f.read().splitlines()

for s in src_strs:
    step_1 = s.replace("ppt{(", "{\"pos\" : [")
    step_2 = step_1.replace("),", "], \"id\" : ")
    step_3 = f"{step_2},"
    print(step_3)
