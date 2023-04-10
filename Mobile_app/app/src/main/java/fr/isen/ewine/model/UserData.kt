package fr.isen.ewine.model

data class UserData(
    val idUser: String,
    val cellarData: ArrayList<CellarData>
    )

data class CellarData (
        var bottleName: String,
        var bottleTypeOfWine: String,
        var bottleProducerName : String,
        var bottleYearOfProduction : String,
        var bottleYearOfExpiration : String
        )