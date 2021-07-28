import org.jetbrains.kotlin.gradle.tasks.KotlinCompile

plugins {
    id("org.jetbrains.kotlin.jvm") version "1.3.61"
    application
    id("org.openjfx.javafxplugin") version "0.0.8"
}

repositories {
    jcenter()
    mavenCentral()
}

dependencies {
    // Align versions of all Kotlin components
//    implementation(platform("org.jetbrains.kotlin:kotlin-bom"))

    // Use the Kotlin JDK 8 standard library.
    implementation("org.jetbrains.kotlin:kotlin-stdlib-jdk8")
    testImplementation("org.jetbrains.kotlin:kotlin-test")
    testImplementation("org.jetbrains.kotlin:kotlin-test-junit")

    compile("no.tornado:tornadofx:1.7.19")

}

javafx {
    modules("javafx.controls")
}

application {
    // Define the main class for the application
    mainClassName = "satellite.MyAppKt"
}

val compileKotlin: KotlinCompile by tasks
compileKotlin.kotlinOptions {
    jvmTarget = "11"
}
val compileTestKotlin: KotlinCompile by tasks
compileTestKotlin.kotlinOptions {
    jvmTarget = "11"
}