import os
import shutil

def generate(path, indx):
    task_name, answer_name = "task_{}".format(indx), "answer_{}".format(indx)


def main():
    path = "task_first_year"
    if os.path.exists(path):
        shutil.rmtree(path)
    os.mkdir(path)


    return 0






if __name__ == '__main__':
    main()