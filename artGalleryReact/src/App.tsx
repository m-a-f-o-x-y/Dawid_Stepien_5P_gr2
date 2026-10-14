import SideBar from "./components/SideBar/SideBar.tsx";
import "./App.css";
import { useEffect, useState, useRef } from "react";

function App({initial_id}) {
  let [currentArtworkData, setArtworkData] = useState({});
  let ImageID = useRef("");
  let [ArtworkID, setArtworkID] = useState(initial_id);
  let [imagePointer, setImagePointer] = useState({});

  let HTTPheader = {
    "Accept-Encoding": "gzip, deflate, br, zstd",
    "Accept-Language": "en-US,en;q=0.9",
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7",
    "Cache-Control": "max-age=0",
    "Content-Type": "application/json"
  }

  // funkcja fetchująca dane z bazy danych
  useEffect(() => {
    fetch(`https://api.artic.edu/api/v1/artworks/${ArtworkID}?fields=id,image,image_id,thumbnail,title,date_display,description`
      , { method: "POST", headers: HTTPheader })
      .then((res) => res.json())
      .then((data) => {
        setArtworkData(data.data);
        ImageID = data.data.image_id;

        fetch(`https://www.artic.edu/iiif/2/${ImageID}/full/843,/0/default.png`, {
          method: "GET",
          referrer: "https://www.artic.edu",
          headers:
            { 
              "AIC-User-Agent": "Mozilla/5.0 (iPhone; CPU iPhone OS 17_0 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.0 Mobile/15E148 Safari/604.1",
            }
        }
        )
          .then((res) => res.json())
          .then((data) => { setImagePointer(data) })

      }
      )
      .catch(() => { console.log("Error while loading data...") });

    //fetch do ilustracji

  }, [ArtworkID]);

  return (
    <>
      <div id="gallery_body">
        <SideBar eventRandomize={() => { setArtworkID((Math.round(Math.random() * 100000 + 1))) }} />
        <img id="gallery_img" 
        src={`https://www.artic.edu/iiif/2/${ImageID}/full/843,/0/default.png`} 
        data-testid="image"/>
        <div id="gallery_content">
          <div id="gallery_layout">
            <div id="item_header">
              <h2 id="item_title" data-testid="title">{currentArtworkData.title}</h2>
              <div id="item_year_wrapper">
                <span id="item_year">{currentArtworkData.date_display}</span>
              </div>
            </div>
            <p id="item_content" data-testid="description"> {currentArtworkData.description}</p>
          </div>
        </div>
      </div>
    </>
  );
}

export default App;
