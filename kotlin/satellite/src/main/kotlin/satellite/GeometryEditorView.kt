package satellite

import javafx.stage.FileChooser
import tornadofx.*

class GeometryEditorView : View() {
    private val myController: MyController by inject()
    private val model: CellDetectorModel by inject()
    private val fileChooser = FileChooser().apply {
        title = "Open GDML file"
        extensionFilters.addAll(
            FileChooser.ExtensionFilter("GDML Files", "*.gdml"),
            FileChooser.ExtensionFilter("All Files", "*.*")
        )

    }
    override val root = hbox {

        gridpane {
            row {
                vbox {
                    button("Load GDML") {
                        action {
                            fileChooser.title = "Open GDML file"
                            val file = fileChooser.showOpenDialog(this@GeometryEditorView.primaryStage)
                            if (file != null) {
                                myController.messege.set("Open file: $file")
                            }
                        }
                    }
                    button("Export GDML") {
                        action {
                            fileChooser.title = "Save as GDML file"
                            val fileForSave = fileChooser.showSaveDialog(this@GeometryEditorView.primaryStage)
                            if (fileForSave != null) {
                                myController.messege.set("Save to file: $fileForSave")
                            }
                        }
                    }
                }

            }
            row {
                hbox {


                    form {
                        fieldset("Cell") {
                            field("Thickness") {
                                textfield(model.thinckness)
                            }
                            field("Material") {
                                textfield()
                            }
                            field("Radius") {
                                textfield()
                            }
                        }
                        fieldset("Case") {
                            hiddenWhen(myController.caseDisable)
                            field("Thinckess") {
                                textfield()
                            }
                            field("Material") {
                                textfield()
                            }
                        }
                        button("Insert front") {
                            action {
                                model.commit {
                                    val cell = model.item
                                    myController.messege.set("Add cell: $cell")
                                }
                            }
                        }
                        button("Insert end") {}
                        button("Insert above") {}
                        button("Insert belove") {}
                    }
                }


            }

        }
    }
}