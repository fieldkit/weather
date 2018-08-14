@Library('conservify') _

conservifyProperties()

timestamps {
    node () {
        conservifyBuild(name: 'weather')
        distributeFirmware()
    }

    refreshDistribution()
}
