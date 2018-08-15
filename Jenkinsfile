@Library('conservify') _

conservifyProperties()

timestamps {
    node () {
        conservifyBuild(name: 'weather', archive: true)
        distributeFirmware()
    }

    refreshDistribution()
}
