@Library('conservify') _

conservifyProperties()

timestamps {
    node () {
        conservifyBuild(name: 'weather')

        build job: "distribution", wait: false
    }
}
