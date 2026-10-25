import "./GenericButton.css"

function GenericButton({imgSource, clicked})
{
return (
    <button onClick={clicked}>
        <img src={imgSource} alt="" />
    </button>
);
}

export default GenericButton;