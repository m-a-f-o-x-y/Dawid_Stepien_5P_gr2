const config = {
  testEnvironment: 'jest-environment-jsdom',
  transform: {
    '^.+\\.(js|jsx|ts|tsx)$': 'babel-jest',
  },
    "jest":{
     "moduleNameMapper": {
      "\\.(jpg|jpeg|png|gif|eot|otf|webp|svg|ttf|woff|woff2|mp4|webm|wav|mp3|m4a|aac|oga|css)$": "<rootDir>/__mocks__/fileMock.js",
      "\\.(css|less|scss|sass)$": "identity-obj-proxy"
    },
  }
};

export default config;