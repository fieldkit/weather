@Library('conservify') _

timestamps {
    node () {
        conservifyBuild(name: 'weather', repository: 'https://github.com/fieldkit/weather.git')

        build job: "distribution", parameters: []
    }
}
