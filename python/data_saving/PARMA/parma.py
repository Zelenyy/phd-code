import os
from odf.opendocument import OpenDocumentSpreadsheet
from odf.table import Table, TableColumn, TableRow, TableCell
from odf.text import P

PARMA_DIR = "/home/zelenyy/storage/Загрузки/parma_cpp"


def convert_FFPtable_USO(path):
    name = "FFPtable.uso"
    table  = Table(name=name)
    with open(path) as fin:
        for i in range(2):
            table.addElement(TableColumn())

        for line in fin.readlines():
            line = line.split()
            row = TableRow()
            for item in line:
                cell = TableCell(valuetype="float", value=item)
                row.addElement(cell)
            table.addElement(row)
    return table

def convert_FFPtable_day(path):
    name = "FFPtable.day"

    table  = Table(name=name)
    with open(path) as fin:
        line = fin.readline().split()[2:]
        row = TableRow()
        for year in line:
            table.addElement(TableColumn())
            cell = TableCell(valuetype="float", value=year)
            row.addElement(cell)
        table.addElement(row)

        for line in fin.readlines():
            line = line.split()[2:]
            row = TableRow()
            for item in line:
                cell = TableCell(valuetype="float", value=item)
                row.addElement(cell)
            table.addElement(row)
    return table






def main():
    input_dir = os.path.join(PARMA_DIR, "input")
    doc = OpenDocumentSpreadsheet()
    table_list = []
    table_list.append(convert_FFPtable_day(os.path.join(input_dir, "FFPtable.day")))
    table_list.append(convert_FFPtable_USO(os.path.join(input_dir, "FFPtable.uso")))
    for table in table_list:
        doc.spreadsheet.addElement(table)
    doc.save("parma.ods")

if __name__ == '__main__':
    main()