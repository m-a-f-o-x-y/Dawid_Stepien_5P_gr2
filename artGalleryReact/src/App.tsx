import SideBar from "./components/SideBar/SideBar.tsx";
import "./App.css";
import { useEffect, useState } from "react";

function App() {
  let [currentArtworkData, setArtworkData] = useState({});
  let [ImageID, setImageID] = useState();
  let [ArtworkID, setArtworkID] = useState(24645);
  let [ImageSize, setImageSize] = useState(); // pobieramy z siostrzanego api dostępny rozmiar dla obrazu

  useEffect(() => {
    fetch(`https://api.artic.edu/api/v1/artworks/${ArtworkID}?fields=id,image,image_id,thumbnail,title,date_display,description`)
      .then((res) => res.json())
      .then((data) => {
        setArtworkData(data.data);
        setImageID(data.data.image_id);
      }
      )
      .catch(() => { console.log("Error while loading data...") });

  }, [ArtworkID]);

  return (
    <>
      <div id="gallery_body">
        <SideBar />
        <div id="gallery_img" 
        style={{backgroundImage: `url(https://www.artic.edu/iiif/2/${ImageID}/full/1670,/0/default.jpg)`}}/>
        <div id="gallery_content">
          <div id="gallery_layout">
            <div id="item_header">
              <h2 id="item_title">{currentArtworkData.title}</h2>
              <span id="item_year">{currentArtworkData.date_display}</span>
            </div>
            <p id="item_content"> {currentArtworkData.description}</p>
          </div>
        </div>
      </div>
    </>
  );
}

export default App;
