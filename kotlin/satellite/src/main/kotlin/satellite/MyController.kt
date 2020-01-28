package satellite

import javafx.beans.property.SimpleBooleanProperty
import javafx.beans.property.SimpleDoubleProperty
import javafx.beans.property.SimpleObjectProperty
import javafx.beans.property.SimpleStringProperty
import tornadofx.*

class MyController : Controller() {
    val messege = SimpleStringProperty()
    val caseDisable = SimpleBooleanProperty(false)
    val simType =
        SimpleObjectProperty<SimulationType>(
            SimulationType.front
        )

    val particleList = mutableListOf<Particle>(Particle()).asObservable()

}

enum class SimulationType {
    front,
    angle,
    forthPi,
    all
}

data class Particle(
    val minEnergy: SimpleDoubleProperty = SimpleDoubleProperty(),
    val maxEnergy: SimpleDoubleProperty = SimpleDoubleProperty(),
    val stepEnergy: SimpleDoubleProperty = SimpleDoubleProperty()
)

data class CellDetecor(
    val thinckness: SimpleDoubleProperty = SimpleDoubleProperty()
)

class CellDetectorModel : ItemViewModel<CellDetecor>(CellDetecor()) {
    val thinckness = bind { item?.thinckness }
}