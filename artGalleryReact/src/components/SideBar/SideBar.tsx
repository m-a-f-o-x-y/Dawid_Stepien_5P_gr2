import "./SideBar.css";
import GenericButton from "../GenericButton/GenericButton";
import logo from "../../assets/logo.jpg";
import arrowIcon from "../../assets/icons/svg/arrow.svg";
import diceIcon from "../../assets/icons/svg/dice.svg";
import { useState } from "react";

function SideBar() {
    let [isVisible, setVisibility] = useState(true);


        // zawsze wrapować w funkcję wszelkie funkcje zmieniające state'a!!!!!!!111!!!11!!
    return (
        <>
            <aside style={isVisible ? {visibility: "visible"} : {visibility: "collapse"} }>
                <img src={logo} alt="" />
                <div id="sidebar_buttons">
                    <GenericButton imgSource={arrowIcon} clicked={() => {setVisibility(false)}}/> 
                    <GenericButton imgSource={diceIcon} clicked={null}/>
                </div>
                
            </aside>
        </>
    );

}

export default SideBar;