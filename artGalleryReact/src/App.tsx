import SideBar from "./components/SideBar/SideBar.tsx";
import "./App.css";

function App() {
  return (
    <>
      <div id="gallery_body">
        <SideBar />

        <img src="{null}" alt="" />
        <div id="gallery_content">
          <h2 id="item_title">Lorem, ipsum.</h2>
          <p id="item_content"> bla bla bla...</p>
        </div>
      </div>
    </>
  );
}

export default App;
