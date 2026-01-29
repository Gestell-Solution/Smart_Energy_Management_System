allprojects {
    repositories {
        google()
        mavenCentral()
    }
}

val newBuildDir: Directory =
    rootProject.layout.buildDirectory
        .dir("../../build")
        .get()
rootProject.layout.buildDirectory.value(newBuildDir)

subprojects {
    val newSubprojectBuildDir: Directory = newBuildDir.dir(project.name)
    project.layout.buildDirectory.value(newSubprojectBuildDir)
    
    project.evaluationDependsOn(":app")
    
    // Force compatible AndroidX core version for ALL subprojects
    project.configurations.all {
        resolutionStrategy {
            force("androidx.core:core:1.12.0")
            force("androidx.core:core-ktx:1.12.0")
            
            eachDependency {
                if (requested.group == "androidx.core") {
                    useVersion("1.12.0")
                }
                if (requested.group == "androidx.appcompat") {
                    useVersion("1.6.1")
                }
                if (requested.group == "androidx.activity") {
                    useVersion("1.8.0")
                }
                if (requested.group == "androidx.fragment") {
                    useVersion("1.6.2")
                }
                if (requested.group == "androidx.lifecycle") {
                    useVersion("2.7.0")
                }
            }
        }
    }
    
    // Fix for plugins missing namespace (AGP 8.0+ requirement)
    if (project.name == "flutter_bluetooth_serial") {
        project.afterEvaluate {
             val android = project.extensions.findByName("android") as? com.android.build.gradle.BaseExtension
             if (android != null && android.namespace == null) {
                 android.namespace = "io.github.edufolly.flutterbluetoothserial"
             }
        }
    }
    if (project.name == "device_info_plus") {
        project.afterEvaluate {
             val android = project.extensions.findByName("android") as? com.android.build.gradle.BaseExtension
             if (android != null && android.namespace == null) {
                 android.namespace = "dev.fluttercommunity.plus.device_info"
             }
        }
    }
}
subprojects {
    project.evaluationDependsOn(":app")
}

tasks.register<Delete>("clean") {
    delete(rootProject.layout.buildDirectory)
}
