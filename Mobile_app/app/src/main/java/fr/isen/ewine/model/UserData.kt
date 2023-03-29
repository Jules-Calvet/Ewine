package fr.isen.ewine.model

data class UserData (
    val id_user: String,
    val cellar_data: CellarData,

){
    data class CellarData (
        var bottle_name: String,
        var bottle_TypeOfWine: String,
        var bottle_ProducerName : String,
        var bottle_YearOfProduction : String,
        var bottle_YearOfExpiration : String
    )
}