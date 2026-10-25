import { describe, it } from "node:test";
import { render } from "@testing-library/react";
import App from "./App";

describe (App, () => {
    it("isRenderingProperly", () => {
        const {getByTestId} = render(<App initial_id={24645}/>);
        const titleContent = getByTestId("title").textContent;

        expect(titleContent).not.toHaveLength(0);
    })
    if ("GoodOutputFromRandomization", () => {
        const {getByTestId, getByRole} = render(<App initial_id={24645}/>);
        
    })
    it("isImageVisible", () => {
        const {getByTestId} = render(<App initial_id={24645}/>);
        const imgContent = getByTestId("image");
    })
})