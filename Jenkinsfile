@Library('conservify') _

conservifyProperties()

timestamps {
    node () {
        conservifyBuild(name: 'weather', archive: "build/firmware/module/*.bin")
        distributeFirmware(directory: "build/firmware/module")
    }

    refreshDistribution()
}
